// Surface.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColdEye.h"
#include "Surface.h"

#include "Pattern\MsgSquare.h"


#include "H264Play.h"

#include "Database\DBShadow.h"
#include "Com\Communication.h"

int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser)
{
	CSurface* pSurface = (CSurface*)dwUser;
	
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

	// �������Ҫ����
	if (m_BindedCamera->m_LocalConfig.IsActivate) {
		//�����ԭ���ǹر�״̬����
		if (this->m_hRealPlay == 0) {
			Print("Config--Acivate: off-->on");
			ConnectRealPlay();
			StartRealPlay();
		}

		//��Ƶ�洢��Ҫ���� 
		if (m_BindedCamera->m_LocalConfig.IsVideoRecordEnabled) {
			//��Ƶ�洢ԭ���ǹر�״̬����
			if (!m_bIsRecording) {
				Print("Config--Record: off-->on");
				StartAutoRecord();
			}
		}

		//�Զ�������Ҫ�����������������Զ�����ʱ��
		if (ShouldWatch()) {
			//��δ��ʼ�Զ���������
			if (!m_bIsWatching) {
				// �����Զ����������ı�����Ϣ
				Print("Config--Watch: off-->on");
				StartAutoWatch();
			}
		}
	}
	//�������Ҫ�ر�
	else {
		//�����ԭ���ǿ���״̬��ر������
		if (this->m_hRealPlay > 0) {
			Print("Going to off");
			//��������Զ�������ֹͣ�Զ�����
			if (m_bIsWatching) {
				//Print();
				StopAutoWatch();
			}

			//�������¼����ֹͣ¼��
			if (m_bIsRecording) {

			}

			//�ر������(����)
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




/**@brief ����ʵʱ��Ƶ
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
		CCommunication::GetInstance()->Alarm(m_BindedCamera);//������ͷ���ͱ�����Ϣ
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

	CCommunication::GetInstance()->OverAlarm(this->m_BindedCamera);//������ͷ�������������� �����ر�����

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
	ON_MESSAGE(USER_MSG_CAMERA_CONFIG_OO_CHANGE, &CSurface::OnUserMsgCameraConfigOoChange)
	ON_MESSAGE(USER_MSG_CAMERA_CONFIG_RD_CHANGE, &CSurface::OnUserMsgCameraConfigRdChange)
	ON_MESSAGE(USER_MSG_CAMERA_CONFIG_AW_CHANGE, &CSurface::OnUserMsgCameraConfigAwChange)
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



int CSurface::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	mReverseBtn.Create(_T("���ŷ�"), WS_CHILD, {0,0,0,0}, this, 1 );
	mReverseBtn.ShowWindow(SW_HIDE);

	mDelBtn.Create(_T("ɾ��"), WS_CHILD, {0,0,0,0}, this, 2);
	mDelBtn.ShowWindow(SW_HIDE);

	return 0;
}


void CSurface::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ����ר�ô����/����û���
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
			else if (lParam == (LPARAM)&mDelBtn) {

				// if (IDOK)

				OnCameraLogOff();
			}
			break;

	}
	return 0;
}


afx_msg LRESULT CSurface::OnUserMsgCameraConfigOoChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CSurface::OnUserMsgCameraConfigRdChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CSurface::OnUserMsgCameraConfigAwChange(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
