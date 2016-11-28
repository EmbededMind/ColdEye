
// ColdEyeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColdEye.h"
#include "ColdEyeDlg.h"
#include "afxdialogex.h"
#include "Com\SerialPort.h"
#include "Com\Communication.h"
#include "Dbt.h"
#include "ExHardDrive\ExHardDrive.h"

#include "Database\DBShadow.h"
#include "Database\DBLogger.h"
//��������ͷ�ļ�
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

														// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CColdEyeDlg �Ի���



CColdEyeDlg::CColdEyeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLDEYE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



void CColdEyeDlg::UpdateLayout()
{
	CRect  rClient;
	GetClientRect(rClient);

	m_pTitleBk_left[0] = { 0,0 };
	m_pTitleBk_left[1] = { 450,0 };
	m_pTitleBk_left[2] = { 400,100 };
	m_pTitleBk_left[3] = { 0,100 };

	m_pTitleBk_center[0] = { m_pTitleBk_left[1].x,0 };
	m_pTitleBk_center[1] = { m_pTitleBk_left[1].x + 800,0 };
	m_pTitleBk_center[2] = { m_pTitleBk_left[2].x + 800,100 };
	m_pTitleBk_center[3] = { m_pTitleBk_left[2].x,100 };

	m_pTitleBk_right[0] = { m_pTitleBk_center[1].x,0 };
	m_pTitleBk_right[1] = { m_pTitleBk_center[1].x + 300,0 };
	m_pTitleBk_right[2] = { m_pTitleBk_center[2].x + 300,100 };
	m_pTitleBk_right[3] = { m_pTitleBk_center[2].x,100 };

	LONG   titleHeight = rClient.Height() / 10;
	LONG   tipTextHeight  = titleHeight / 3;

	m_rTitle.left  = rClient.left;
	m_rTitle.top   = rClient.top;
	m_rTitle.right = rClient.right;
	m_rTitle.bottom = m_rTitle.top + titleHeight;

	m_rSysTimeText.left  = 40;
	m_rSysTimeText.right = m_rSysTimeText.left + 341;

	m_rSysTimeText.top  = 37;
	m_rSysTimeText.bottom  = m_rSysTimeText.top + 39;


	m_rAwTipText.left  = 704;
	m_rAwTipText.right = m_rAwTipText.left+ 250;
	m_rAwTipText.top  = 37;
	m_rAwTipText.bottom = m_rAwTipText.top + 39;

	m_rHwTipText.left = 1226;
	m_rHwTipText.right = m_rHwTipText.left  + 250;
	m_rHwTipText.top = 37;
	m_rHwTipText.bottom = m_rHwTipText.top + 39;


	mWall.SetWindowPos(NULL, rClient.left, rClient.top + titleHeight, rClient.Width(), rClient.Height()-titleHeight-titleHeight/5, 0);
}


void CColdEyeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColdEyeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(USER_MSG_SCAN_DEV, &CColdEyeDlg::OnUserMsgScanDev)
	ON_WM_SIZE()
	ON_MESSAGE(WM_COMM_RXDATA, &CColdEyeDlg::OnCommReceive)
	ON_MESSAGE(USER_MSG_RECORDVOICE, &CColdEyeDlg::OnRecordVoice)
	ON_WM_DEVICECHANGE()
	ON_WM_TIMER()
	         //UEER_MSG_CAMERA_CONFIG_CHANGE
	ON_MESSAGE(USER_MSG_CAMERA_CONFIG_CHANGE, &CColdEyeDlg::OnUserMsgCameraConfigChange)
END_MESSAGE_MAP()


// CColdEyeDlg ��Ϣ�������

BOOL CColdEyeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mWall.Create(IDD_WALL, this);
	((CColdEyeApp*)AfxGetApp())->SetWallDlg(&mWall);
	mWall.ShowWindow(SW_SHOW);

	mMenu.Create(m_hWnd, _T("MenuWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, {0,0,0,0});
	mMenu.ShowWindow(false);


	SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
	mWall.SetFocus();

	if (CSerialPort::GetInstance(COM_KB)->InitPort(this, COM_KB, 9600, 'N', 8, 1, EV_RXCHAR, 512))
	{
		CSerialPort::GetInstance(COM_KB)->StartMonitoring();
	}
	else
	{
		AfxMessageBox(_T("COM_KB û�з��ִ��ڻ򴮿ڱ�ռ��!"));
	}
	if (CSerialPort::GetInstance(COM_CAM)->InitPort(this, COM_CAM, 9600, 'N', 8, 2, EV_RXCHAR, 512))
	{
		CSerialPort::GetInstance(COM_CAM)->StartMonitoring();
	}
	else
	{
		AfxMessageBox(_T("COM_CAM û�з��ִ��ڻ򴮿ڱ�ռ��!"));
	}
	if (CExHardDrive::GetInstance()->ScanDisk(this))
	{
		CExHardDrive::GetInstance()->Updata();
	}


	CDBShadow::GetInstance()->BroadcaseInitFileMsg();

	m_AwTipText  = _T("�Զ���������");
	m_HwTipText  = _T("�ѹرջؼҿ���");


	SetTimer(TIMER_ID_SECOND_TICK, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CColdEyeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CColdEyeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		//dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CPaintDC dc(this);
		CRect rClient;
		GetClientRect(rClient);

		//dc.FillSolidRect(&rClient, RGB(171, 130, 255));
		PaintTitle(&dc);

		CFont font;
		font.CreatePointFont(240, _T("������ͤ�к�"));

		CPen* pOldPen;
		CFont* pOldFont;
		pOldFont = dc.SelectObject(&font);

		dc.SetTextColor(0xFFFFFF);
		dc.FillSolidRect(m_rSysTimeText, RGB(0, 0, 0));
		dc.Rectangle(m_rSysTimeText);
		dc.Rectangle(m_rAwTipText);
		dc.Rectangle(m_rHwTipText);
		 
		dc.TextOutW(m_rSysTimeText.left + 1, m_rSysTimeText.top+1, m_SysTime.Format(_T("%Y-%m-%d %H:%M:%S")));


		dc.FillSolidRect(m_rAwTipText, RGB(58, 58, 58));
		if (((CColdEyeApp*)AfxGetApp())->m_SysConfig.auto_watch_status) {
			dc.TextOutW(m_rAwTipText.left + 1, m_rAwTipText.top + 1, _T("�Զ������ѿ���"));
		}
		else{
			dc.TextOutW(m_rAwTipText.left + 1, m_rAwTipText.top + 1, _T("�Զ������ѹر�"));
		}

		dc.FillSolidRect(m_rHwTipText, RGB(0, 0, 0));
		dc.SetTextColor(0x7ED321);
		dc.TextOutW(m_rHwTipText.left + 1, m_rHwTipText.top + 1, _T("δ�����ؼҿ���"));

		dc.SelectObject(pOldFont);

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CColdEyeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CColdEyeDlg::OnUserMsgScanDev(WPARAM wParam, LPARAM lParam)
{
	Print("Find %d camera", wParam);
	if (wParam > 1) {
		ASSERT(FALSE);
	}
	for (int i = 0; i < wParam; i++) {
		static bool hasPostLogin = false;
		CCamera* pCamera = new CCamera();
		pCamera->SetCommonNetConfig(&((SDK_CONFIG_NET_COMMON_V2*)lParam)[i]);
		PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_LOGIN, 0, (LPARAM)pCamera);
		if (hasPostLogin) {
			Print("What a fuck! Has post login");
			ASSERT(FALSE);
		}
		hasPostLogin = true;
	}
	return 0;
}

BOOL CColdEyeDlg::SetVolumeLevel(int type)
{
	HRESULT hr;
	IMMDeviceEnumerator* pDeviceEnumerator = 0;
	IMMDevice *pDevice = 0;
	IAudioEndpointVolume* pAudioEndpointVolume = 0;
	IAudioClient* pAudioClient = 0;

	try {
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
		if (FAILED(hr)) throw "CoCreateInstance";
		hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
		if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
		hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
		if (FAILED(hr)) throw "pDevice->Active";
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if (FAILED(hr)) throw "pDevice->Active";

		if (type == 1) {
			hr = pAudioEndpointVolume->SetMute(FALSE, NULL);
			if (FAILED(hr)) throw "SetMute";
		}
		else if (type == 0) {
			hr = pAudioEndpointVolume->SetMute(TRUE, NULL);
			if (FAILED(hr)) throw "SetMute";
		}
		else {
			if (type <= 1 || type >= 4) {
				hr = E_INVALIDARG;
				throw "Invalid Arg";
			}

			float fVolume;
			fVolume = type / 100.0f;
			hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);
			if (FAILED(hr)) throw "GetMasterVolumeLevelScalar";
			if (type == 2)
			{
				fVolume += 0.1;
				if (fVolume > 1) fVolume = 1;
			}
			else if (type == 3)
			{
				fVolume -= 0.1;
				if (fVolume < 0) fVolume = 0;
			}
			hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &GUID_NULL);
			if (FAILED(hr)) throw "SetMasterVolumeLevelScalar";

			pAudioClient->Release();
			pAudioEndpointVolume->Release();
			pDevice->Release();
			pDeviceEnumerator->Release();
			return true;
		}
	}
	catch (...) {
		if (pAudioClient) pAudioClient->Release();
		if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
		if (pDevice) pDevice->Release();
		if (pDeviceEnumerator) pDeviceEnumerator->Release();
		throw;
	}
	return 0;
}


BOOL CColdEyeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_CONTEXTMENU) {
		if (mWall.IsWindowVisible()) {
			mWall.ShowWindow(false);
			mMenu.ShowWindow(true);

		}
		else {
			mWall.ShowWindow(true);
			mMenu.ShowWindow(false);
			mWall.SetFocus();
		}
		
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


void CColdEyeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (IsWindow(mWall.m_hWnd)) {
		UpdateLayout();
	}
}

LONG CColdEyeDlg::OnCommReceive(WPARAM pData, LPARAM port)
{
	static int KBmessage_NO = 0;
	static int CAmessage_NO = 0;
	if (port == COM_KB)
	{
		onedata *p = (onedata*)pData;
		printf("COM_KEYBD message NO.%d : ", KBmessage_NO);
		KBmessage_NO++;
		for (int i = 0; i < p->num; i++)
		{
			printf("kb = %d\n", p->ch[i]);
			switch (p->ch[i])
			{
			case KB_MENU:
				keybd_event(VK_APPS, 0, 0, 0);
				keybd_event(VK_APPS, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_UP:
				keybd_event(VK_UP, 0, 0, 0);
				keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_BACK:
				keybd_event(VK_BACK, 0, 0, 0);
				keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_AUTOWATCH:

				break;
			case KB_LEFT:
				keybd_event(VK_LEFT, 0, 0, 0);
				keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_ENTER:
				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_RIGHT:
				keybd_event(VK_RIGHT, 0, 0, 0);
				keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_UDISK:
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('U', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('U', 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_VOLUP:
				this->SetVolumeLevel(ENLARGE_VOLUME);
				break;
			case KB_DOWN:
				keybd_event(VK_DOWN, 0, 0, 0);
				keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_BRIUP:
				break;
			case KB_FUNC:
				this->SetFocus();
				this->SetActiveWindow();
				break;
			case KB_VOLDOWN:
				this->SetVolumeLevel(REDUCE_VOLUME);
				break;
			case KB_TALKQUIET:
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('S', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_BRIDOWN:
				break;
			case KB_SWITCH:
			{
				char str[120] = "shutdown -f -s -t 0";
				system(str);
				break;
			}
			case KB_PTTDOWN:
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('T', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('T', 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_PTTUP:
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('O', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('O', 0, KEYEVENTF_KEYUP, 0);
				break;
			default:
				break;
			}
		}
	}

	else if (port == COM_CAM)
	{
		static int cnt = 0;
		onedata *p = (onedata*)pData;

		printf("COM_CAMERA message NO.%d : ", CAmessage_NO);
		CAmessage_NO++;
		for (int i = 0; i < p->num; i++) {
			printf("%02X ", p->ch[i]);
		}
		printf("\n");

		if (p->num != 17)
		{
			printf("message length <17\n");
			return 0;
		}
		if (p->ch[0] != 0x24)
		{
			printf("message head !=0x24\n");
			return 0;
		}

		//�����ж�CRC
		//.....

		if (p->ch[1] == 0x02 && p->ch[2] == 0x01)
		{
			switch (p->ch[3])
			{
			case 0x01:
				CCommunication::GetInstance()->RecSetVolumeProc(p->ch);
				break;
			case 0x02:
				switch (p->ch[4])
				{
				case 1:
					CCommunication::GetInstance()->RecTalkProc(p->ch);
					break;
				case 2:
					CCommunication::GetInstance()->RecYouTalkProc(p->ch);
					break;
				case 3:
					CCommunication::GetInstance()->RecOverTalkProc(p->ch);
					break;
				default:
					break;
				}
				break;
			case 0x03:
				CCommunication::GetInstance()->ReplyTalk(p->ch);
				break;
			case 0x04:
				if (p->ch[4] == 1)
				{
					CCommunication::GetInstance()->RecAlarmProc(p->ch);
				}
				if (p->ch[4] == 2)
				{
					CCommunication::GetInstance()->RecOverAlarmProc(p->ch);
				}
				break;
			case 0x05:
				CCommunication::GetInstance()->RecHandleProc(p->ch);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

BOOL CColdEyeDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	int i = 0;
	CString flag;
	DWORD mask;
	TCHAR diskname[5];
	DEV_BROADCAST_VOLUME* pDisk;
	switch (nEventType)
	{
	case DBT_DEVICEARRIVAL:
		pDisk = (DEV_BROADCAST_VOLUME*)dwData;
		mask = pDisk->dbcv_unitmask;
		for (i = 0; i < 32; i++)
		{
			if ((mask >> i) == 1)
			{
				diskname[0] = 'A' + i;
				diskname[1] = '\0';
				wcscat_s(diskname, TEXT(":\\"));
				break;
			}
		}
		flag.Format(_T("%s"), diskname);
		CExHardDrive::GetInstance()->Init(this, flag);
		CExHardDrive::GetInstance()->StartMonitoring();
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		pDisk = (DEV_BROADCAST_VOLUME*)dwData;
		mask = pDisk->dbcv_unitmask;
		for (i = 0; i < 32; i++)
		{
			if ((mask >> i) == 1)
			{
				diskname[0] = 'A' + i;
				diskname[1] = '\0';
				wcscat_s(diskname, TEXT(":\\"));
				break;
			}
		}
		flag.Format(_T("%s"), diskname);
		CExHardDrive::GetInstance()->EndThread();
		break;
	}
	return 0;
}

afx_msg LRESULT CColdEyeDlg::OnRecordVoice(WPARAM wParm, LPARAM lParm)
{
	Print("begin record");
	mMessageBox->SendMessage(USER_MSG_RECORDVOICE, NULL, NULL);
	return 0;
}


CMyMenuWnd& CColdEyeDlg::GetMyMenu()
{
	return mMenu;
}


void CColdEyeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��������
	static int i = 0;
	m_SysTime  = CTime::GetCurrentTime();
	CDBLogger::GetInstance()->LogSystemTime(m_SysTime);
	

	InvalidateRect(m_rSysTimeText);
	InvalidateRect(m_rAwTipText);
	InvalidateRect(m_rHwTipText);
	CDialogEx::OnTimer(nIDEvent);
}

void CColdEyeDlg::PaintTitle(CPaintDC *dc)
{
	CBrush brush_left(RGB(0, 0, 0));
	CBrush* oldBrush = dc->SelectObject(&brush_left);
	dc->Polygon(m_pTitleBk_left, 4);
	brush_left.DeleteObject();

	CBrush brush_center(RGB(58, 58, 58));
	dc->SelectObject(&brush_center);
	dc->Polygon(m_pTitleBk_center, 4);
	brush_center.DeleteObject();

	CBrush brush_right(RGB(0, 0, 0));
	dc->SelectObject(&brush_right);
	dc->Polygon(m_pTitleBk_right, 4);
	brush_right.DeleteObject();
}


afx_msg LRESULT CColdEyeDlg::OnUserMsgCameraConfigChange(WPARAM wParam, LPARAM lParam)
{
	CPort* pPort  = (CPort*)wParam;
	if (pPort) {
		DeviceConfig* pConfig  = (DeviceConfig*)lParam;
		if (pPort->m_Id) {
			// �� m_Id �Ŷ˿ڷ��� ���� �������
			Print("Set %d camera vol:%d", pPort->m_Id, pConfig->Volumn);
		}
	}

	return 0;
}
