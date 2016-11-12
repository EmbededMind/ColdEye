#include "stdafx.h"

#include "ColdEye.h"

#include "UISurface.h"

#include "H264Play.h"

#include "Database\DBShadow.h"



int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurfaceUI* pDataChnl = (CSurfaceUI*)dwUser;
	BOOL ret  = H264_PLAY_InputData(pDataChnl->m_lPlayPort, (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);

	if (!ret) {
		TRACE("InputData failed:%d\n", H264_PLAY_GetLastError(1));
	}

	if (pDataChnl->m_pAlmFile && pDataChnl->m_pAlmFile->m_hFile != CFile::hFileNull) {
		pDataChnl->m_pAlmFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	if (pDataChnl->m_pRdFile && pDataChnl->m_pRdFile->m_hFile != CFile::hFileNull) {
		pDataChnl->m_pRdFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}


	return 1;
}


IMPLEMENT_DUICONTROL(CSurfaceUI)

CSurfaceUI::CSurfaceUI()
{
	m_hWnd = 0;
	m_pAlmFile = NULL;
}


CSurfaceUI::CSurfaceUI(CWnd* pMFCWnd)
{
	if (pMFCWnd != NULL  &&  IsWindow(pMFCWnd->m_hWnd)) {
		m_hWnd = pMFCWnd->m_hWnd;
	}
}


CSurfaceUI::~CSurfaceUI()
{

}


void CSurfaceUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
		{
			m_pParent->DoEvent(event);
		}
		else
		{
			CControlUI::DoEvent(event);
		}

		return;
	}

	switch (event.Type)
	{
	case UIEVENT_SETFOCUS:
		break;
	case UIEVENT_KILLFOCUS:
		break;
	case UIEVENT_TIMER:
		switch (event.wParam)
		{
			case TIMER_ID_AUTO_RECORD:
				TRACE("Record package\n");
				PackageReocrdFile();
				break;
			//------------------------------------
			case TIMER_ID_ALARM:
				if (m_wAlarmStamp > 0) {
					m_wAlarmStamp--;
				}

				if (m_wAlarmStamp == 0) {
					OnAlarmStop();
				}
				break;
			//------------------------------------
			case TIMER_ID_AUTO_WATCH:
				TRACE("End of watch\n");
				// TODO
				break;
			//------------------------------------
			case TIMER_ID_RECONNECT:
				PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_RELOGIN, 0, (LPARAM)m_BindedCamera );
				break;
		}
		break;
	case UIEVENT_KEYDOWN:
		TRACE("Surface case key\n");
		switch (event.chKey)
		{
		case VK_UP:

			break;
		case VK_DOWN:

			break;
		case VK_LEFT:

			break;
		case VK_RIGHT:

			break;
		case VK_MENU:

			break;

		default:

			break;
		}
		break;

	default:
		//TRACE("Surface event type:%d\n", event.Type);
		break;
	}

	CControlUI::DoEvent(event);
}


LPCTSTR CSurfaceUI::GetClass() const
{
	return _T("CSurfaceUI");
}


LPVOID CSurfaceUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("Surface")) == 0)
	{
		return static_cast<CSurfaceUI*>(this);
	}

	return CControlUI::GetInterface(pstrName);
}



void CSurfaceUI::SetInternVisible(bool bVisible)
{
	__super::SetInternVisible(bVisible);
	::ShowWindow(m_hWnd, bVisible);
}


void CSurfaceUI::SetBorderColor(DWORD dwColor)
{
	m_dwBorderColor = dwColor;
}


DWORD CSurfaceUI::GetBorderColor()
{
	return m_dwBorderColor;
}



void CSurfaceUI::SetFocusedBorderColor(DWORD dwColor)
{
	m_dwFocusedBorderColor = dwColor;
}



DWORD CSurfaceUI::GetFocusedBorderColor()
{
	return m_dwFocusedBorderColor;
}


void CSurfaceUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CControlUI::SetAttribute(pstrName, pstrValue);
}


void CSurfaceUI::SetPos(RECT rc)
{
	//__super::SetPos(rc);
	CControlUI::SetPos(rc);
	if (m_hWnd) {
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
}

void CSurfaceUI::SetFixedPos(RECT rc)
{
	SetFixedXY({rc.left, rc.top});
	SetFixedWidth(rc.right - rc.left);
	SetFixedHeight(rc.bottom - rc.top);
	if (m_hWnd) {
		::SetWindowPos(m_hWnd, NULL, rc.left+2, rc.top+2, rc.right - rc.left-4, rc.bottom - rc.top-4, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}




BOOL CSurfaceUI::Attach(HWND hWndNew)
{
	if (!::IsWindow(hWndNew))
	{
		return FALSE;
	}

	this->m_hWnd = hWndNew;
	return TRUE;
}


void CSurfaceUI::BindCamera(CCamera* pCamera)
{
	m_BindedCamera = pCamera;

	CString subDir;
	subDir.Format(_T("%d\\"), m_BindedCamera->m_Id);

	m_RecordFileButler.SetDirection(_T(NORMAL_RECORD_PATH) + subDir);
	m_AlarmFileButler.SetDirection(_T(ALARM_RECORD_PATH) + subDir);

	
	m_RecordFileButler.SetFileType(1);
	m_RecordFileButler.Attach(CDBShadow::GetInstance());

	//m_AlarmFileMgr.Attach(CDBShadow::GetInstance());
	m_AlarmFileButler.SetFileType(2);
	m_AlarmFileButler.Attach(CDBShadow::GetInstance());
}


/**@brief 根据本地配置开启或关闭相关功能
 *
 */
void CSurfaceUI::ExecuteLocalConfig()
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


/**@brief 更改本地配置
 *
 */
void CSurfaceUI::ExecuteLocalConfig(LocalConfig* pConfig)
{
	ASSERT(m_BindedCamera != NULL);
}


 /**@brief 判断此刻是否处在自动看船时段
  *
  */
BOOL CSurfaceUI::ShouldWatch()
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



HWND CSurfaceUI::Detach()
{
	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}




/**@brief 播放实时视频
 *
 */
void CSurfaceUI::ConnectRealPlay()
{
	BYTE byFileHeadBuf;
	H264_PLAY_GetPort(&m_lPlayPort);


	if (H264_PLAY_OpenStream(m_lPlayPort, &byFileHeadBuf, 1, SOURCE_BUF_MIN * 100)) {		
		if (!H264_PLAY_SetStreamOpenMode(m_lPlayPort, STREAME_REALTIME)) {
			TRACE("Play set stream open mode failed:%d\n", H264_PLAY_GetLastError(m_lPlayPort));
		}

		if (m_hWnd > 0) {
			if (!H264_PLAY_Play(m_lPlayPort, m_hWnd)) {
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
void CSurfaceUI::DisconnectRealPlay()
{
	if (m_hRealPlay > 0) {
		H264_DVR_DelRealDataCallBack_V2(m_hRealPlay, cbRealData, (long)this);

		if (!H264_DVR_StopRealPlay(m_hRealPlay)) {
			TRACE("StopRealPlay failed:%d\n", H264_DVR_GetLastError());
		}
		else {
			m_hRealPlay = 0;
		}

		H264_PLAY_Stop(m_lPlayPort);
		H264_PLAY_CloseStream(m_lPlayPort);
		H264_PLAY_FreePort(m_lPlayPort);
		m_lPlayPort = 0;

		CWnd::FromHandle(this->GetWindowHandle())->Invalidate();
	}
}




HWND CSurfaceUI::GetWindowHandle()
{
	return m_hWnd;
}




void CSurfaceUI::StartAlarmRecord(CFile* pFile)
{
	ASSERT(pFile  &&  pFile->m_hFile != CFile::hFileNull);
	m_pAlmFile = pFile;
}



void CSurfaceUI::StopAlarmRecord()
{

	m_bIsAlarming = false;
	m_pAlmFile = NULL;
}


void CSurfaceUI::StartRecord(CFile* pf)
{
	if (m_lPlayPort > 0) {
		m_pRdFile = pf;
	}
}



void CSurfaceUI::StopRecord()
{
	TRACE("Stop record\n");
	m_pRdFile = NULL;
}



BOOL CSurfaceUI::StartAutoRecord()
{
	m_bIsRecording = true;

	CFile* pf = m_RecordFileButler.AllocRecordFile();
	if (pf) {
		StartRecord(pf);
		SetTimer(TIMER_ID_AUTO_RECORD, 30*1000);
		return TRUE;
	}
	else {
		Print("Alloc file failed when start auto record");
	}
	return FALSE;
}



void CSurfaceUI::StopAutoRecord()
{
	this->KillTimer(TIMER_ID_AUTO_RECORD);
	StopRecord();
	m_RecordFileButler.ReleaseRecordFile();
	m_bIsRecording = false;
}



BOOL CSurfaceUI::StartAutoWatch()
{
	CTime time = CTime::GetCurrentTime();
	UINT minute = time.GetHour() * 60 + time.GetMinute();
	UINT elapse = 0;	

	if (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd < minute) {
		elapse = (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd + 24 * 60 - minute) * 60 * 1000;		
	}
	else if(m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd > minute){
		elapse = (m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd - minute) * 60 * 1000;
	}
	else {
		return FALSE;
	}

	m_BindedCamera->SubscribeAlarmMessage();
	m_bIsWatching = true;

	SetTimer(TIMER_ID_AUTO_WATCH, elapse);
	TRACE("elapse:%d before end:%d:%0d\n", elapse, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd / 60, m_BindedCamera->m_LocalConfig.AutoWatchTimeEnd % 60);
	return TRUE;
}



void CSurfaceUI::StopAutoWatch()
{
	this->KillTimer(TIMER_ID_AUTO_WATCH);
	m_BindedCamera->UnsubscribeAlarmMessage();
	m_bIsWatching = false;
}



void CSurfaceUI::PackageReocrdFile()
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

void CSurfaceUI::OnAlarmTrigged()
{

	m_wAlarmStamp = ALARM_TIMEOUT_CNT;

	if (!m_bIsAlarming) {
		m_bIsAlarming = true;
Print("Alarm trigged");

		CFile* pf  = m_AlarmFileButler.AllocRecordFile();
		if (pf) {
			StartAlarmRecord(pf);
			SetTimer(TIMER_ID_ALARM, 1000);
		}
		else {
			Print("Alloc file failed when alarm trigged");
		}
	}
}


void CSurfaceUI::OnAlarmStop()
{
	m_bIsAlarming = false;

Print("Alarm stop");

	m_wAlarmStamp = 0;
	KillTimer(TIMER_ID_ALARM);
	//StopAlarmRecord();
	m_pAlmFile = NULL;
	m_AlarmFileButler.ReleaseRecordFile();
}



void CSurfaceUI::OnDisConnect()
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

	SetTimer(TIMER_ID_RECONNECT,  10*1000);
}


void CSurfaceUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	CControlUI::DoPaint(hDC, rcPaint);
	CRenderEngine::DrawText(hDC, m_pManager, m_rcItem, _T("gggggg"), 0xD1EEEE, \
		-1, DT_CENTER| DT_VCENTER | DT_SINGLELINE);
}





void CSurfaceUI::PaintBorder(HDC hDC)
{
	if (m_bFocused)
	{
		CRenderEngine::DrawRect(hDC, m_rcItem, 2, 0xFFFF1493);
	}
	else
	{
		CRenderEngine::DrawRect(hDC, m_rcItem, 2, 0xFFEEB422);
	}
}



