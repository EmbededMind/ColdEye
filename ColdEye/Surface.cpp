// Surface.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColdEye.h"
#include "Surface.h"


#include "H264Play.h"

#include "Database\DBShadow.h"


int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurface* pSurface = (CSurface*)dwUser;

	BOOL bRet = H264_PLAY_InputData(pSurface->m_lPlayPort, (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);

	if (!bRet) {
		TRACE("InputData failed:%d\n", H264_PLAY_GetLastError(pSurface->m_lPlayPort));
	}

	if (pSurface->m_pAlmFile  &&  pSurface->m_pAlmFile->m_hFile != CFile::hFileNull) {
		pSurface->m_pAlmFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	if (pSurface->m_pRdFile  &&  pSurface->m_pRdFile->m_hFile != CFile::hFileNull) {
		pSurface->m_pRdFile->Write((unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);
	}

	return 1;
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

	m_RecordFileButler.SetFileType(1);
	m_RecordFileButler.Attach(CDBShadow::GetInstance());

	m_AlarmFileButler.SetFileType(2);
	m_AlarmFileButler.Attach(CDBShadow::GetInstance());
}



void CSurface::ExecuteLocalConfig()
{
	ASSERT(m_BindedCamera != NULL);

	// �������Ҫ����
	if (m_BindedCamera->m_LocalConfig.IsActivate) {
		//�����ԭ���ǹر�״̬����
		if (this->m_hRealPlay == 0) {
			Print("Config--Acivate: false-->true");
			ConnectRealPlay();
		}

		//��Ƶ�洢��Ҫ���� 
		if (m_BindedCamera->m_LocalConfig.IsVideoRecordEnabled) {
			//��Ƶ�洢ԭ���ǹر�״̬����
			if (!m_bIsRecording) {
				//StartRecord("E:\\Record\\normal\\test.h264");
				StartAutoRecord();
			}
		}

		//�Զ�������Ҫ�����������������Զ�����ʱ��
		if (ShouldWatch()) {
			//��δ��ʼ�Զ���������
			if (!m_bIsWatching) {
				// �����Զ����������ı�����Ϣ
				StartAutoWatch();
			}
		}
	}
	//�������Ҫ�ر�
	else {
		//�����ԭ���ǿ���״̬��ر������
		if (this->m_hRealPlay > 0) {

			//��������Զ�������ֹͣ�Զ�����
			if (m_bIsWatching) {

			}

			//�������¼����ֹͣ¼��
			if (m_bIsRecording) {

			}

			//�ر������(����)
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




/**@brief ����ʵʱ��Ƶ
*
*/
void CSurface::ConnectRealPlay()
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



/**@brief  �ر������(����)
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
END_MESSAGE_MAP()



// CSurface ��Ϣ�������

void CSurface::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
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
	// TODO: �ڴ˴������Ϣ����������
}


void CSurface::OnKillFocus(CWnd* pNewWnd)
{
	mHasFocused = FALSE;
	CWnd::OnKillFocus(pNewWnd);
	OnNcPaint();
	// TODO: �ڴ˴������Ϣ����������
}


void CSurface::OnNcPaint()
{
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnNcPaint()
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
