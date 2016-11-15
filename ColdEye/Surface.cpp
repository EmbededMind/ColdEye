// Surface.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "Surface.h"


#include "H264Play.h"

#include "Database\DBShadow.h"


int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurface* pSurface = (CSurface*)dwUser;
	
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

	return 1;
}



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



void CSurface::BindCamera(CCamera* pCamera)
{
	m_BindedCamera = pCamera;

	CString subDir;

	subDir.Format(_T("%d\\"), m_BindedCamera->m_Id);

	m_RecordFileButler.SetDirection(_T(NORMAL_RECORD_PATH) + subDir);
	m_AlarmFileButler.SetDirection(_T(ALARM_RECORD_PATH) + subDir);

	m_RecordFileButler.SetFileType(RECORD_NORMAl);
	m_RecordFileButler.Attach(CDBShadow::GetInstance());

	m_AlarmFileButler.SetFileType(RECORD_ALARM);
	m_AlarmFileButler.Attach(CDBShadow::GetInstance());


	mOsdPainter.SetBitmap( &((CColdEyeApp*)AfxGetApp())->m_Bitmap );
}



void CSurface::ExecuteLocalConfig()
{
	ASSERT(m_BindedCamera != NULL);

	// 摄像机需要开启
	if (m_BindedCamera->m_LocalConfig.IsActivate) {
		//摄像机原本是关闭状态则开启
		if (this->m_hRealPlay == 0) {
			Print("Config--Acivate: off-->on");
			ConnectRealPlay();
			StartRealPlay();
		}

		//视频存储需要开启 
		if (m_BindedCamera->m_LocalConfig.IsVideoRecordEnabled) {
			//视频存储原本是关闭状态则开启
			if (!m_bIsRecording) {
				Print("Config--Record: off-->on");
				StartAutoRecord();
			}
		}

		//自动看船需要开启并且现在属于自动看船时段
		if (ShouldWatch()) {
			//还未开始自动看船则开启
			if (!m_bIsWatching) {
				// 开启自动看船，订阅报警消息
				Print("Config--Watch: off-->on");
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
				//Print();
				StopAutoWatch();
			}

			//如果正在录像则停止录像
			if (m_bIsRecording) {

			}

			//关闭摄像机(画面)
			StopRealPlay();
			DisconnectRealPlay();
		}
	}
}



void CSurface::ExecuteLocalConfig(LocalConfig* pConfig)
{
	ASSERT(m_BindedCamera != NULL);
}


BOOL CSurface::ShouldWatch()
{
	if (m_BindedCamera == NULL) {
		return FALSE;
	}

	if (!m_BindedCamera->m_LocalConfig.IsAutoWatchEnabled) {
		return FALSE;
	}

	CTime time = CTime::GetCurrentTime();
	UINT minute = time.GetHour() * 60 + time.GetMinute();

	if (minute < m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd) {
		if (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd < m_BindedCamera->m_LocalConfig.AutoWatchTimeStart
			|| minute >= m_BindedCamera->m_LocalConfig.AutoWatchTimeStart) {
			return TRUE;
		}
	}

	return FALSE;
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
			if (!H264_PLAY_Play(m_lPlayPort, mSurface.m_hWnd)) {
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




void CSurface::StopRealPlay()
{
	m_bIsRealPlaying = false;

	H264_PLAY_Stop(m_lPlayPort);
}





void CSurface::StartAlarmRecord(CFile* pFile)
{
	ASSERT(pFile && pFile->m_hFile != CFile::hFileNull);
	m_pAlmFile = pFile;
}



void CSurface::StopAlarmRecord()
{
	m_bIsAlarming = false;
	m_pAlmFile = NULL;
}


void CSurface::StartRecord(CFile* pf)
{
	if (m_lPlayPort > 0) {
		m_pRdFile = pf;
	}
}


void CSurface::StopRecord()
{
	TRACE("Stop record\n");
	m_pRdFile = NULL;
}



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


void CSurface::StopAutoRecord()
{
	KillTimer(TIMER_ID_AUTO_RECORD);

	StopRecord();

	m_RecordFileButler.ReleaseRecordFile();
	m_bIsRecording = false;
}



BOOL  CSurface::StartAutoWatch()
{
	CTime  time = CTime::GetCurrentTime();
	UINT minute = time.GetHour() * 60 + time.GetMinute();
	UINT elapse = 0;

	if (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd < minute) {
		elapse = (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd + 24 * 60 - minute) * 60 * 1000;
	}
	else if (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd > minute) {
		elapse = (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd - minute) * 60 * 1000;
	}
	else {
		return FALSE;
	}

	m_BindedCamera->SubscribeAlarmMessage();
	m_bIsWatching = true;

	SetTimer(TIMER_ID_AUTO_WATCH, elapse, NULL);
	TRACE("elapse:%d before end:%d:%0d\n", elapse, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd / 60, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd % 60);
	return TRUE;
}



void CSurface::StopAutoWatch()
{
	KillTimer(TIMER_ID_AUTO_WATCH);
	m_BindedCamera->UnsubscribeAlarmMessage();
	m_bIsWatching = false;
}



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


void CSurface::OnAlarmTrigged()
{
	m_wAlarmStamp = ALARM_TIMEOUT_CNT;

	if (!m_bIsAlarming) {
		m_bIsAlarming = true;
		Print("Alarm trigged");

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



void CSurface::OnAlarmStop()
{
	m_bIsAlarming = false;

	Print("Alarm stop");

	m_wAlarmStamp = 0;
	KillTimer(TIMER_ID_ALARM);

	m_pAlmFile = NULL;
	m_AlarmFileButler.ReleaseRecordFile();
}




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





void CSurface::OnReconnect()
{
	ConnectRealPlay();
	ExecuteLocalConfig();
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
		ExecuteLocalConfig();
		KillTimer(TIMER_ID_RECONNECT);
	}
	return 0;
}



int CSurface::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	mSurface.Create(NULL, _T("Surface"), WS_CHILD | WS_VISIBLE, {0,0,0,0}, this, 1);
	mSurface.ShowWindow(SW_SHOW);

	mControlWnd.Create(NULL, _T("ControlPanel"), WS_OVERLAPPED | WS_VISIBLE, {0,0,0,0}, this, 2);
	mControlWnd.ShowWindow(SW_SHOW);

	return 0;
}


void CSurface::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (IsWindow(mControlWnd)) {
		CRect rClient;
		GetClientRect(rClient);

		long surface_height = rClient.Height() * 4 / 5;
		long control_height = rClient.Height() / 5;

		mSurface.SetWindowPos(NULL, rClient.left, rClient.top, rClient.Width(), surface_height, 0);
		mControlWnd.SetWindowPos(NULL, rClient.left, rClient.bottom-control_height, rClient.Width(), control_height, 0);
	}
}
