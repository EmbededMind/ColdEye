// CSurfaceHolder.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "SurfaceHolder.h"

#include "H264Play.h"
#include "Database\DBShadow.h"

int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurfaceHolder* pHolder = (CSurfaceHolder*)dwUser;

	BOOL bRet = H264_PLAY_InputData(pHolder->m_lPlayPort, (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);

	if (!bRet) {
		TRACE("InputData failed:%d\n", H264_PLAY_GetLastError(pHolder->m_lPlayPort));
	}

	if (pHolder->m_pAlmFile  &&  pHolder->m_pAlmFile->m_hFile != CFile::hFileNull) {
		pHolder->m_pAlmFile->Write( (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	if (pHolder->m_pRdFile  &&  pHolder->m_pRdFile->m_hFile != CFile::hFileNull) {
		pHolder->m_pRdFile->Write( (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	return 1;
}





// CSurfaceHolder

IMPLEMENT_DYNAMIC(CSurfaceHolder, CDialogEx)

CSurfaceHolder::CSurfaceHolder(CWnd* pParent)
	:CDialogEx(IDD_HOLDER, pParent)
{
	m_pAlmFile = NULL;
	m_pRdFile = NULL;
}

CSurfaceHolder::~CSurfaceHolder()
{
}


void CSurfaceHolder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


void CSurfaceHolder::BindCamera(CCamera* pCamera)
{
	m_BindedCamera = pCamera;

	CString subDir; 

	subDir.Format(_T("%d\\"), m_BindedCamera->m_Id);

	m_RecordFileButler.SetDirection(_T(NORMAL_RECORD_PATH) + subDir);
	m_AlarmFileButler.SetDirection(_T(ALARM_RECORD_PATH) + subDir);

	m_RecordFileButler.SetFileType(1);
	m_RecordFileButler.Attach(CDBShadow::GetInstance());

	m_AlarmFileButler.SetFileType(2);
	m_AlarmFileButler.Attach(CDBShadow::GetInstance());
}



void CSurfaceHolder::ExecuteLocalConfig()
{
	ASSERT(m_BindedCamera != NULL);

	// 摄像机需要开启
	if (m_BindedCamera->m_LocalConfig.IsActivate) {
		//摄像机原本是关闭状态则开启
		if (this->m_hRealPlay == 0) {
			ConnectRealPlay();
		}

		//视频存储需要开启 
		if (m_BindedCamera->m_LocalConfig.IsVideoRecordEnabled) {
			//视频存储原本是关闭状态则开启
			if (!m_bIsRecording) {
				//StartRecord("E:\\Record\\normal\\test.h264");

				StartAutoRecord();
			}
		}

		//自动看船需要开启并且现在属于自动看船时段
		if (ShouldWatch()) {
			//还未开始自动看船则开启
			if (!m_bIsWatching) {
				// 开启自动看船，订阅报警消息
				StartAutoWatch();
			}
		}
	}
	//摄像机需要关闭
	else {
		//摄像机原本是开启状态则关闭摄像机
		if (this->m_hRealPlay > 0) {

			//如果正在自动看船则停止自动看船
			if (m_bIsWatching) {

			}

			//如果正在录像则停止录像
			if (m_bIsRecording) {

			}

			//关闭摄像机(画面)
			DisconnectRealPlay();
		}
	}
}


void CSurfaceHolder::ExecuteLocalConfig(LocalConfig* pConfig)
{
	ASSERT(m_BindedCamera != NULL);
}



BOOL CSurfaceHolder::ShouldWatch()
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
void CSurfaceHolder::ConnectRealPlay()
{
	BYTE byFileHeadBuf;
	H264_PLAY_GetPort(&m_lPlayPort);


	if (H264_PLAY_OpenStream(m_lPlayPort, &byFileHeadBuf, 1, SOURCE_BUF_MIN * 100)) {
		if (!H264_PLAY_SetStreamOpenMode(m_lPlayPort, STREAME_REALTIME)) {
			TRACE("Play set stream open mode failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
		}

		if (mSurface.m_hWnd > 0) {
			if (!H264_PLAY_Play(m_lPlayPort, mSurface.m_hWnd)) {
				TRACE("Play failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
			}
		}
	}
	else {
		TRACE("Open stream failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
	}


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
void CSurfaceHolder::DisconnectRealPlay()
{
	if (m_hRealPlay > 0) {
		H264_DVR_DelRealDataCallBack_V2(m_hRealPlay, cbRealData, (long)this);

		if (!H264_DVR_StopRealPlay(m_hRealPlay)) {
			TRACE("StopRealPlay failed:%d\n", H264_DVR_GetLastError());
		}
		//else {
		//	m_hRealPlay = 0;
		//}

		H264_PLAY_Stop(m_lPlayPort);
		H264_PLAY_CloseStream(m_lPlayPort);
		H264_PLAY_FreePort(m_lPlayPort);
		m_lPlayPort = 0;

		Print("After disconnect hRealPlay:%d", m_hRealPlay);

		Invalidate();
	}
}



void CSurfaceHolder::StartAlarmRecord(CFile* pFile)
{
	ASSERT(pFile && pFile->m_hFile != CFile::hFileNull);
	m_pAlmFile = pFile;
}



void CSurfaceHolder::StopAlarmRecord()
{
	m_bIsAlarming = false;
	m_pAlmFile = NULL;
}


void CSurfaceHolder::StartRecord(CFile* pf)
{
	if (m_lPlayPort > 0) {
		m_pRdFile = pf;
	}
}


void CSurfaceHolder::StopRecord()
{
	TRACE("Stop record\n");
	m_pRdFile = NULL;
}



BOOL CSurfaceHolder::StartAutoRecord()
{
	m_bIsRecording = true;

	CFile* pf = m_RecordFileButler.AllocRecordFile();
	if (pf) {
		StartRecord(pf);
		SetTimer(TIMER_ID_AUTO_RECORD, 30 * 1000, NULL);
		return TRUE;
	}
	else {
		Print("Alloc file failed when start auto record");
	}

	return FALSE;
}


void CSurfaceHolder::StopAutoRecord()
{
	KillTimer(TIMER_ID_AUTO_RECORD);

	StopRecord();

	m_RecordFileButler.ReleaseRecordFile();
	m_bIsRecording = false;
}



BOOL  CSurfaceHolder::StartAutoWatch()
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

	SetTimer(TIMER_ID_AUTO_WATCH , elapse,  NULL);
	TRACE("elapse:%d before end:%d:%0d\n", elapse, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd / 60, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd % 60);
	return TRUE;
}



void CSurfaceHolder::StopAutoWatch()
{
	KillTimer(TIMER_ID_AUTO_WATCH);
	m_BindedCamera->UnsubscribeAlarmMessage();
	m_bIsWatching = false;
}


void CSurfaceHolder::PackageRecordFile()
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


void CSurfaceHolder::OnAlarmTrigged()
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




void CSurfaceHolder::OnAlarmStop()
{
	m_bIsAlarming = false;

	Print("Alarm stop");

	m_wAlarmStamp = 0;
	KillTimer(TIMER_ID_ALARM);

	m_pAlmFile = NULL;
	m_AlarmFileButler.ReleaseRecordFile();
}



void CSurfaceHolder::OnDisconnect()
{
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

	SetTimer(TIMER_ID_RECONNECT, 10*1000, NULL);
}


void CSurfaceHolder::OnReconnect()
{
	ConnectRealPlay();
	ExecuteLocalConfig();
}



BEGIN_MESSAGE_MAP(CSurfaceHolder, CDialogEx)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CSurfaceHolder 消息处理程序






BOOL CSurfaceHolder::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_UP:
			Print("Caee VK_UP");
			break;
		case VK_DOWN:
			Print("Case VK_DOWN");
			break;
		case VK_LEFT:
			Print("Case VK_LEFT");
			break;
		case VK_RIGHT:
			Print("Case VK_RIGHT");
			break;
		}
	}


	return CWnd::PreTranslateMessage(pMsg);
}


void CSurfaceHolder::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	mIsSelected = FALSE;

	// TODO: 在此处添加消息处理程序代码
	Print("Holders kill focus--%d", this->m_hWnd);
}


void CSurfaceHolder::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	mIsSelected = TRUE;

	// TODO: 在此处添加消息处理程序代码
	Print("Holders get focus--%d", this->m_hWnd);
}


BOOL CSurfaceHolder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rClient;
	GetClientRect(&rClient);

	mSurface.Create(NULL, NULL, WS_CHILDWINDOW, CRect(rClient.left,rClient.top,rClient.right,rClient.bottom), this, 0);
	mSurface.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSurfaceHolder::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rClient;

	GetClientRect(&rClient);

	if (IsWindow(mSurface.m_hWnd)) {
		mSurface.SetWindowPos(NULL, rClient.left, rClient.top, rClient.Width(), rClient.Height(), 0);
	}
}


void CSurfaceHolder::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnNcPaint()
	if (!mIsSelected) {
		return;
	}

	HDC hDC = ::GetWindowDC(m_hWnd);
	CRect rc;
	::GetWindowRect(m_hWnd, &rc);
	rc = CRect(0, 0, rc.right - rc.left, rc.bottom - rc.top);

	::FrameRect(hDC, &rc, ::CreateSolidBrush(GetSysColor(COLOR_WINDOWFRAME)));
	rc.InflateRect(-1, -1);
	::FrameRect(hDC, &rc, ::CreateSolidBrush(GetSysColor(COLOR_WINDOWFRAME)));

	::ReleaseDC(m_hWnd, hDC);
}
