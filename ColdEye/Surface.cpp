// Surface.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "Surface.h"

#include "Pattern\MsgSquare.h"


#include "H264Play.h"

#include "Database\DBShadow.h"
#include "Com\Communication.h"

CMutex mutex_RealData;

/**@brief 实时数据回调
 *
 */
int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurface* pSurface = (CSurface*)dwUser;

mutex_RealData.Lock();
	
	if (mutex != 0) {
		Print("Mutex error:%d", mutex);
		ASSERT(false);
	}

	mutex = 1;

	if (pSurface->m_bIsRealPlaying) {

		BOOL bRet = H264_PLAY_InputData(pSurface->m_lPlayPort, (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);

		if (!bRet) {
			TRACE("InputData failed:%d\n", H264_PLAY_GetLastError(pSurface->m_lPlayPort));
		}
	}

	if (pSurface->m_pAlmFile  &&  pSurface->m_pAlmFile->m_hFile != CFile::hFileNull) {
		pSurface->m_pAlmFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	if (pSurface->m_pRdFile  &&  pSurface->m_pRdFile->m_hFile != CFile::hFileNull) {
		pSurface->m_pRdFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	mutex = 0;

mutex_RealData.Unlock();

	return 1;
}


/**@brief 缺省OSD绘制回调
 *
 */
void CALLBACK cbDefaultDrawOSD(LONG nPort, HDC hDC, LONG nUser)
{
	static bool flag = false;
	CSurface* pSurface = (CSurface*)nUser;
	static BITMAP bmp;

	static CDC mSrcDC;
	static long lImgWidth = 0;
	static long lImgHeight = 0;


	CDC* pDstDC = CDC::FromHandle(hDC);

	if (!flag) {
		flag = true;
		CBitmap& bitmap = ((CColdEyeApp*)AfxGetApp())->m_Bitmap;
		bitmap.GetBitmap(&bmp);

		CImage img;
		img.Load(_T("Icon_Min.png"));


		lImgWidth = img.GetWidth();
		lImgHeight = img.GetHeight();
		

		mSrcDC.CreateCompatibleDC(pDstDC);

		//mSrcDC.SelectObject(img);
		mSrcDC.SelectObject(bitmap);
	}
		pDstDC->BitBlt(10, 500, bmp.bmWidth, bmp.bmHeight, &mSrcDC, 0, 0, NOTSRCCOPY | SRCAND);
		//pDstDC->BitBlt(10, 50, lImgWidth, lImgHeight, &mSrcDC, 0, 0, SRCAND);

		//pDstDC->MoveTo(10, 50);
		//pDstDC->LineTo(10+bmp.bmWidth,  50);
		//pDstDC->LineTo(10+bmp.bmWidth,  50+bmp.bmHeight);

		//pDstDC->MoveTo(10, 50);
		//pDstDC->LineTo(10, 400);

	
	//pSurface->mOsdPainter.ShowBitmap(hDC);

}



// CSurface

IMPLEMENT_DYNAMIC(CSurface, CWnd)

CSurface::CSurface()
{
	RegisterWindowClass();
}

CSurface::~CSurface()
{
}


/**@brief 绑定到指定Port
 *
 */
void CSurface::BindPort(CPort* pPort)
{
	m_BindedPort  = pPort;

	m_BindedCamera  = pPort->m_pCamera;

	CString subDir;
	subDir.Format(_T("%d\\"), m_BindedPort->GetId());

	m_RecordFileButler.SetDirection(_T(NORMAL_RECORD_PATH) + subDir);
	m_AlarmFileButler.SetDirection(_T(ALARM_RECORD_PATH) + subDir);

	m_RecordFileButler.SetFileType(RECORD_NORMAl);
	m_RecordFileButler.Attach(CDBShadow::GetInstance());

	m_AlarmFileButler.SetFileType(RECORD_ALARM);
	m_AlarmFileButler.Attach(CDBShadow::GetInstance());

	m_RecordFileButler.SetOwner(m_BindedPort->m_Id);
	m_AlarmFileButler.SetOwner(m_BindedPort->m_Id);


	mOsdPainter.SetBitmap(&((CColdEyeApp*)AfxGetApp())->m_Bitmap);
}


/**@brief 绑定一个摄像头
 *
 */
//void CSurface::BindCamera(CCamera* pCamera)
//{
//	m_BindedCamera = pCamera;
//
//	CString subDir;
//
//	subDir.Format(_T("%d\\"), m_BindedCamera->m_Id);
//
//	m_RecordFileButler.SetDirection(_T(NORMAL_RECORD_PATH) + subDir);
//	m_AlarmFileButler.SetDirection(_T(ALARM_RECORD_PATH) + subDir);
//
//	m_RecordFileButler.SetFileType(RECORD_NORMAl);
//	m_RecordFileButler.Attach(CDBShadow::GetInstance());
//
//	m_AlarmFileButler.SetFileType(RECORD_ALARM);
//	m_AlarmFileButler.Attach(CDBShadow::GetInstance());
//
//	m_RecordFileButler.SetOwner(m_BindedCamera->m_Id);
//	m_AlarmFileButler.SetOwner(m_BindedCamera->m_Id);
//
//
//	mOsdPainter.SetBitmap( &((CColdEyeApp*)AfxGetApp())->m_Bitmap );
//}



void CSurface::ExecuteConfig()
{
	ASSERT(m_BindedPort != NULL);

	//摄像机需要开启
	if (m_BindedPort->m_DevConfig.IsCameraOn) {
	    // 摄像机原本是关闭状态则开启
		if (this->m_hRealPlay == 0) {
			Print("Config--Active: off-->on");
			ConnectRealPlay();
			StartRealPlay();
		}

		//视频存储需要开启
		if (m_BindedPort->m_DevConfig.IsRecordEnabled) {
			//视频原本是关闭状态则开启
			if (!m_bIsRecording) {
				Print("Config--Record: off-->on");
				StartAutoRecord();
			}
		}

		//自动看船需要开启且现在属于自动看船时段
		if (m_BindedPort->m_DevConfig.IsAutoWatchEnabled) {
			//自动看船是关闭状态则开启
			if (!m_bIsAutoWatchEnabled) {
				StartAutoWatch();
			}
		}
	}
	
	//摄像机需要关闭
	else {
		//摄像机原本是开启状态则关闭摄像机
		if (this->m_hRealPlay > 0) {
			Print("Going to off");
			//如果正在自动看船则停止自动看船
			if (m_bIsWatching) {
				StopAutoWatch();
			}

			//如果正在录像则停止录像
			if (m_bIsRecording) {

			}

			//关闭摄像机（画面）
			StopRealPlay();
			DisconnectRealPlay();
		}
	}
}



BOOL CSurface::ShouldWatch(CTime& refTime)
{
	UINT minute = refTime.GetHour() * 60 + refTime.GetMinute();


	if (m_BindedPort->m_AwConfig.End < m_BindedPort->m_AwConfig.Begining) {
		if (minute < m_BindedPort->m_AwConfig.End || minute >= m_BindedPort->m_AwConfig.Begining) {
			return TRUE;
		}
	}
	else if (m_BindedPort->m_AwConfig.End > m_BindedPort->m_AwConfig.Begining) {
		if (minute >= m_BindedPort->m_AwConfig.Begining && minute < m_BindedPort->m_AwConfig.End) {
			return TRUE;
		}
	}
	return FALSE;
}


/**@brief 判断是否需要开启看船
 *
 */
BOOL CSurface::ShouldWatch()
{
	if (m_BindedPort == NULL || m_BindedPort->m_pCamera == NULL) {
		return FALSE;
	}

	if (!m_BindedPort->m_DevConfig.IsAutoWatchEnabled) {
		return FALSE;
	}

	CTime time = CTime::GetCurrentTime();
	return ShouldWatch(time);
}


UINT CSurface::GetNextWatchEventElapse(CTime& refTime)
{
	UINT elapse  = 0;
	
	UINT second  = refTime.GetHour()*3600 + refTime.GetMinute() * 60 + refTime.GetSecond();

	UINT begin_sec  = m_BindedPort->m_AwConfig.Begining*60;
	UINT end_sec    = m_BindedPort->m_AwConfig.End* 60;

	if (m_bIsWatching) {
		if ( end_sec < second) {
			elapse  = end_sec + 24*3600 - second;
		}
		else {
			elapse  = end_sec - second;
		}
	}
	else {
		if (begin_sec < second) {
			elapse  = 24*3600-second+begin_sec;
		}
		else {
			elapse = begin_sec  -second;
		}
	}

	return elapse;
}

/**@brief 播放实时视频
*
*/
void CSurface::ConnectRealPlay()
{
	H264_DVR_CLIENTINFO playstru;
	playstru.nChannel = 0;
	playstru.nStream = 1;
	playstru.nMode = 0;

	m_hRealPlay = H264_DVR_RealPlay(m_BindedCamera->GetLoginId(), &playstru);

	if (m_hRealPlay > 0) {
		H264_DVR_SetRealDataCallBack_V2(m_hRealPlay, cbRealData, (long)this);
	}
	else {
		TRACE("RealPlay failed:%d\n", H264_DVR_GetLastError());
	}
}



/**@brief  关闭摄像机(画面)
*
*/
void CSurface::DisconnectRealPlay()
{
	if (m_hRealPlay > 0) {
		H264_DVR_DelRealDataCallBack_V2(m_hRealPlay, cbRealData, (long)this);

		if (!H264_DVR_StopRealPlay(m_hRealPlay)) {
			TRACE("StopRealPlay failed:%d\n", H264_DVR_GetLastError());
		}
		//else {
		//	m_hRealPlay = 0;
		//}
		m_hRealPlay = 0;

		H264_PLAY_Stop(m_lPlayPort);
		H264_PLAY_CloseStream(m_lPlayPort);
		H264_PLAY_FreePort(m_lPlayPort);
		m_lPlayPort = 0;

		Invalidate();
	}
}


/**@brief 打开播放器通道，使之处于播放状态
 *
 */
void CSurface::StartRealPlay()
{
	BYTE byFileHeadBuf;
	H264_PLAY_GetPort(&m_lPlayPort);


	if (H264_PLAY_OpenStream(m_lPlayPort, &byFileHeadBuf, 1, SOURCE_BUF_MIN * 100)) {
		OSD_INFO_TXT osd;
		osd.bkColor = RGB(72, 209, 204);
		osd.color = RGB(0,0,0);
		osd.pos_x = 10;
		osd.pos_y = 80;
		osd.isTransparent = 1;
		osd.isBold = 1;
		strcpy_s(osd.text,  "SealedGhost");

		H264_PLAY_SetOsdTex(m_lPlayPort, &osd);


		H264_PLAY_RigisterDrawFun(m_lPlayPort, cbDefaultDrawOSD, (LONG)this);


		SDK_OSDInfo Osd;
		Osd.index = 1;
		Osd.nChannel = 0;
		Osd.nX = 100;
		Osd.nY = 100;
		
		strcpy_s(Osd.pOSDStr, "LOL");
		long lRet = H264_DVR_SetDevConfig(m_BindedCamera->GetLoginId(), E_SDK_SET_OSDINFO, 0, (char*)&Osd, sizeof(SDK_OSDInfo));


		if (!H264_PLAY_SetStreamOpenMode(m_lPlayPort, STREAME_REALTIME)) {
			TRACE("Play set stream open mode failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
		}

		if (m_hWnd > 0) {
			if (!H264_PLAY_Play(m_lPlayPort, m_hWnd)) {
				TRACE("Play failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
			}
			else {
				m_bIsRealPlaying = true;
			}
		}
	}
	else {
		TRACE("Open stream failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
	}
}



/**@brief 关闭播放器通道
 *
 */
void CSurface::StopRealPlay()
{
	m_bIsRealPlaying = false;

	H264_PLAY_Stop(m_lPlayPort);
}




/**@brief 开始录制报警视频
 *
 */
void CSurface::StartAlarmRecord(CFile* pFile)
{
	ASSERT(pFile && pFile->m_hFile != CFile::hFileNull);
	m_pAlmFile = pFile;
}


/**@brief 停止录制报警视频
 *
 */
void CSurface::StopAlarmRecord()
{
	m_bIsAlarming = false;
	m_pAlmFile = NULL;
}


/**@brief 录像文件赋值，在回调中将视频数据存于文件
 *
 */
void CSurface::StartRecord(CFile* pf)
{
	if (m_lPlayPort > 0) {
		m_pRdFile = pf;
	}
}


/**@brief 录像文件赋 NULL，不存视频数据
 *
 */
void CSurface::StopRecord()
{
	TRACE("Stop record\n");
	m_pRdFile = NULL;
}


/**@brief 开启自动打包录像
 *
 */
BOOL CSurface::StartAutoRecord()
{
	m_bIsRecording = true;

	CFile* pf = m_RecordFileButler.AllocRecordFile();
	if (pf) {
		StartRecord(pf);
		SetTimer(TIMER_ID_AUTO_RECORD, RECORD_PACK_TIMER_ELAPSE * 1000, NULL);
		return TRUE;
	}
	else {
		Print("Alloc file failed when start auto record");
	}

	return FALSE;
}


/**@brief 关闭自动打包录像
 *
 */
void CSurface::StopAutoRecord()
{
	KillTimer(TIMER_ID_AUTO_RECORD);

	StopRecord();

	m_RecordFileButler.ReleaseRecordFile();
	m_bIsRecording = false;
}


void  CSurface::StartWatch()
{
	if (! m_BindedCamera->SubscribeAlarmMessage()) {
		Print("Start watch failed");
		return ;
	}
	m_bIsWatching = true;
}



void CSurface::StopWatch()
{
	if (!m_BindedCamera->UnsubscribeAlarmMessage()) {
		Print("Stop watch failed");
		return ;
	}
	m_bIsWatching = false;

	if (m_bIsAlarming) {
		StopAlarmRecord();
	}
}



/**@brief 自动看船开启
 *
 */
BOOL  CSurface::StartAutoWatch()
{
    m_bIsAutoWatchEnabled   = true;
	CTime  time = CTime::GetCurrentTime();

	if (ShouldWatch(time)) {
		StartWatch();
	}
	else {
		StopWatch();
	}

	UINT elapse   = 0;
	elapse  = GetNextWatchEventElapse(time);

	Print("Next elapse %d--%02d:%02d:%02d", elapse, elapse / 3600, (elapse %3600)/60, elapse % 60);

	SetTimer(TIMER_ID_AUTO_WATCH, elapse*1000, NULL);
	
	return TRUE;
}


/**@brief 自动看船关闭
 *
 */
void CSurface::StopAutoWatch()
{
    m_bIsAutoWatchEnabled  = false;
	KillTimer(TIMER_ID_AUTO_WATCH);
	StopWatch();
}


/**@brief 打包录像文件
 *
 */
void CSurface::PackageRecordFile()
{
	m_pRdFile = NULL;
	m_RecordFileButler.ReleaseRecordFile();

	CFile* pf = m_RecordFileButler.AllocRecordFile();

	if (pf) {
		StartRecord(pf);
	}
	else {
		Print("Alloc null file when package");
	}
}


/**@brief 报警事件发生响应
 *
 */
void CSurface::OnAlarmTrigged()
{
	if (!m_bIsWatching) {
		Print("Alarm trigged at not watch time");
	}

	m_wAlarmStamp = ALARM_TIMEOUT_CNT;

	if (!m_bIsAlarming) {
		m_bIsAlarming = true;
		Print("Alarm trigged");
		CCommunication::GetInstance()->Alarm(m_BindedCamera);//向摄像头发送报警信息
		CFile* pf = m_AlarmFileButler.AllocRecordFile();

		if (pf) {
			StartAlarmRecord(pf);
			SetTimer(TIMER_ID_ALARM, 1000, NULL);
		}
		else {
			Print("Alloc file failed when alarm trigged");
		}
	}
}


/**@brief 报警事件结束响应
 *
 */
void CSurface::OnAlarmStop()
{
	m_bIsAlarming = false;

	Print("Alarm stop");

	CCommunication::GetInstance()->OverAlarm(this->m_BindedCamera);//向摄像头的语音附件发送 报警关闭请求

	m_wAlarmStamp = 0;
	KillTimer(TIMER_ID_ALARM);

	m_pAlmFile = NULL;
	m_AlarmFileButler.ReleaseRecordFile();
}



/**@brief 掉线响应
 *
 */
void CSurface::OnDisconnect()
{
	Print("Surface on disconnect");
	if (m_bIsRecording) {
		StopAutoRecord();
	}

	if (m_bIsAlarming) {
		StopAlarmRecord();
	}

	if (m_bIsWatching) {
		StopAutoWatch();
	}

	DisconnectRealPlay();
	Invalidate();

	m_BindedCamera->OnDisConnnect();

	SetTimer(TIMER_ID_RECONNECT, RECONNECT_TIMER_ELAPSE * 1000, NULL);
}




/**@brief 重连响应
 *
 */
void CSurface::OnReconnect()
{
	ConnectRealPlay();
	//ExecuteLocalConfig();
	ExecuteConfig();
}


/**@brief  摄像头注销响应
 *
 */
void CSurface::OnCameraLogOff()
{
	ASSERT(m_BindedCamera != NULL);

	Print("On camera delete");

	if (m_bIsRecording) {
		StopAutoRecord();
	}

	if (m_bIsAlarming) {
		StopAlarmRecord();
	}

	if (m_bIsWatching) {
		StopAutoWatch();
	}

	DisconnectRealPlay();
	m_BindedCamera->OnDisConnnect();
	Invalidate();

	::SendMessage(GetParent()->m_hWnd, USER_MSG_LOGOFF, 0, (LPARAM)this);
}



BOOL CSurface::RegisterWindowClass(HINSTANCE hInstance)
{
	LPCWSTR className = _T("CSurface");

	WNDCLASS windowclass;

	if (!hInstance) {
		hInstance = AfxGetInstanceHandle();
	}


	if (!(::GetClassInfo(hInstance, className, &windowclass))) {
		windowclass.style = CS_DBLCLKS;
		windowclass.lpfnWndProc = ::DefWindowProc;
		windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
		windowclass.hInstance = hInstance;
		windowclass.hIcon = NULL;
		windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
		windowclass.lpszMenuName = NULL;
		windowclass.lpszClassName = className;

		if (!AfxRegisterClass(&windowclass)) {
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}






BEGIN_MESSAGE_MAP(CSurface, CWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_NCPAINT()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_MESSAGE(USER_MSG_RELOGIN, &CSurface::OnUserMsgRelogin)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(1, &CSurface::OnBnClickedRevsese)
	ON_BN_CLICKED(2, &CSurface::OnBnClickedDelete)
	ON_MESSAGE(USER_MSG_NOFITY_KEYDOWN, &CSurface::OnUserMsgNofityKeydown)
	//ON_MESSAGE(USER_MSG_CAMERA_CONFIG_OO_CHANGE, &CSurface::OnUserMsgCameraConfigOoChange)
	//ON_MESSAGE(USER_MSG_CAMERA_CONFIG_RD_CHANGE, &CSurface::OnUserMsgCameraConfigRdChange)
	//ON_MESSAGE(USER_MSG_CAMERA_CONFIG_AW_CHANGE, &CSurface::OnUserMsgCameraConfigAwChange)
END_MESSAGE_MAP()



// CSurface 消息处理程序

void CSurface::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()
					   //CRect rect;
					   //this->GetClientRect(rect);

					   //dc.MoveTo(0, 0);
					   //dc.LineTo(rect.right, rect.bottom);

}


void CSurface::OnSetFocus(CWnd* pOldWnd)
{
	mHasFocused = TRUE;
	CWnd::OnSetFocus(pOldWnd);
	OnNcPaint();
	// TODO: 在此处添加消息处理程序代码
}


void CSurface::OnKillFocus(CWnd* pNewWnd)
{
	mHasFocused = FALSE;
	CWnd::OnKillFocus(pNewWnd);
	OnNcPaint();
	// TODO: 在此处添加消息处理程序代码
}


void CSurface::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnNcPaint()
	COLORREF border_color = mHasFocused ? RGB(220, 20, 60) : RGB(123, 104, 238);

	HDC hDC = ::GetWindowDC(m_hWnd);
	CRect rc;
	::GetWindowRect(m_hWnd, &rc);
	rc = CRect(0, 0, rc.right - rc.left, rc.bottom - rc.top);

	::FrameRect(hDC, &rc, ::CreateSolidBrush(border_color));
	rc.InflateRect(-1, -1);
	::FrameRect(hDC, &rc, ::CreateSolidBrush(border_color));

	::ReleaseDC(m_hWnd, hDC);
}





void CSurface::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) {
		case TIMER_ID_AUTO_RECORD:
			PackageRecordFile();
			break;
		//----------------------------------------
		case TIMER_ID_ALARM:
			if (m_wAlarmStamp > 0) {
				m_wAlarmStamp--;
			}

			if (m_wAlarmStamp == 0) {
				OnAlarmStop();
			}
			break;
		//----------------------------------------
		case TIMER_ID_AUTO_WATCH:
		    // 到达看船结束时间
			if (m_bIsWatching) {
				 StopWatch();
				 UINT elapse  = GetNextWatchEventElapse(CTime::GetCurrentTime());
				 Print("Next elapse %d--%02d:%02d:%02d", elapse, elapse / 3600, (elapse % 3600) / 60, elapse % 60);
				 SetTimer(TIMER_ID_AUTO_WATCH, elapse, NULL);
			}
			// 来到看船开始时间
			else {
				StartWatch();
				UINT elapse = GetNextWatchEventElapse(CTime::GetCurrentTime());
				Print("Next elapse %d--%02d:%02d:%02d", elapse, elapse / 3600, (elapse % 3600) / 60, elapse % 60);
				SetTimer(TIMER_ID_AUTO_WATCH, elapse*1000, NULL);
			}
			break;
		//----------------------------------------
		case TIMER_ID_RECONNECT:
			PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_RELOGIN, (WPARAM)this->m_hWnd, (LPARAM)m_BindedCamera);
			break;
	}

	CWnd::OnTimer(nIDEvent);
}


afx_msg LRESULT CSurface::OnUserMsgRelogin(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		Print("^_^ ReLogin ^_^");
		ExecuteConfig();
		KillTimer(TIMER_ID_RECONNECT);
	}
	return 0;
}



int CSurface::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	mReverseBtn.Create(_T("倒着放"), WS_CHILD, {0,0,0,0}, this, 1 );
	mReverseBtn.ShowWindow(SW_HIDE);

	mDelBtn.Create(_T("删除"), WS_CHILD, {0,0,0,0}, this, 2);
	mDelBtn.ShowWindow(SW_HIDE);

	return 0;
}


void CSurface::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//if (IsWindow(mControlWnd)) {
	//	CRect rClient;
	//	GetClientRect(rClient);

	//	long surface_height = rClient.Height() * 4 / 5;
	//	long control_height = rClient.Height() / 5;

	//	mSurface.SetWindowPos(NULL, rClient.left, rClient.top, rClient.Width(), surface_height, 0);
	//	mControlWnd.SetWindowPos(NULL, rClient.left, rClient.bottom-control_height, rClient.Width(), control_height, 0);
	//}

	if (IsWindow(mDelBtn)) {
		CRect rClient;
		GetClientRect(rClient);

		long btn_width = rClient.Width() / 5;
		long btn_height = rClient.Height() / 5;

		long margin_left = rClient.Width() / 5;
		long margin_top = rClient.Height() * 2 / 5;


		mReverseBtn.SetWindowPos(NULL, rClient.left + margin_left, rClient.top + margin_top,
			btn_width, btn_height, 0);

		mDelBtn.SetWindowPos(NULL, rClient.left+ margin_left*2 + btn_width, rClient.top + margin_top,
			btn_width, btn_height, 0);
	}
}


BOOL CSurface::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) { 
		CWnd* pWnd = GetFocus();
		CString text;
		pWnd->GetWindowText(text);
		TRACE("--->%S\n", text);

		switch (pMsg->wParam)
		{
			case VK_F8:

				//StopRealPlay();
				H264_PLAY_Pause(this->m_lPlayPort, 1);
				mReverseBtn.ShowWindow(SW_SHOW);
				
				mDelBtn.ShowWindow(SW_SHOW);
				mReverseBtn.SetFocus();
				break;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}



void CSurface::OnBnClickedRevsese()
{
	Print("Reserve Clicked");
}


void CSurface::OnBnClickedDelete()
{
	Print("Delete Clicked");
}

afx_msg LRESULT CSurface::OnUserMsgNofityKeydown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_LEFT:
		case VK_RIGHT:
			if ((LPARAM)&mReverseBtn == lParam) {
				TRACE("Rec reserve btn key msg\n");
				mDelBtn.SetFocus();
			}
			else {
				TRACE("Rec del btn key msg\n");
				mReverseBtn.SetFocus();
			}
			break;
		//---------------------------------------------------
		case VK_BACK:
			mReverseBtn.ShowWindow(SW_HIDE);
			mDelBtn.ShowWindow(SW_HIDE);
			
			this->SetFocus();
			if (m_bIsRealPlaying) {
				H264_PLAY_Pause(m_lPlayPort, 0);
			}
			break;
		//--------------------------------------------------------
		case VK_RETURN:
			//倒着放
			if (lParam == (LPARAM)&(mReverseBtn)) {
				if (m_BindedCamera->m_Param.PictureFlip) {
					m_BindedCamera->m_Param.PictureFlip = 0;
					m_BindedCamera->m_Param.PictureMirror = 0;
				}
				else {
					m_BindedCamera->m_Param.PictureFlip = 1;
					m_BindedCamera->m_Param.PictureMirror = 1;
				}


				MSG msg;
				msg.message = USER_MSG_CAMERA_PARAM;
				msg.lParam = (LPARAM)m_BindedCamera;
				CMsgSquare::GetInstance()->Broadcast(msg);

				PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_CAMERA_PARAM, true, (LPARAM)m_BindedCamera);
			}
			// 删除摄像头
			else if (lParam == (LPARAM)&mDelBtn) {

				// if (IDOK)

				OnCameraLogOff();
			}
			break;

	}
	return 0;
}


// 摄像头开关切换的消息处理
afx_msg LRESULT CSurface::OnUserMsgCameraConfigOoChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


// 摄像头视频存储切换的消息处理
afx_msg LRESULT CSurface::OnUserMsgCameraConfigRdChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


// 摄像头自动看船切换的消息处理
afx_msg LRESULT CSurface::OnUserMsgCameraConfigAwChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
