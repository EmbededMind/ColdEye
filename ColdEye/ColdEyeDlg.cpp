
// ColdEyeDlg.cpp : 实现文件
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
//控制音量头文件
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
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


// CColdEyeDlg 对话框



CColdEyeDlg::CColdEyeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLDEYE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



void CColdEyeDlg::UpdateLayout()
{
	CRect  rClient;
	GetClientRect(rClient);

	LONG   titleHeight = rClient.Height() / 10;
	LONG   tipTextHeight  = titleHeight / 3;



	m_rTitle.left  = rClient.left;
	m_rTitle.top   = rClient.top;
	m_rTitle.right = rClient.right;
	m_rTitle.bottom = m_rTitle.top + titleHeight;

	m_rSysTimeText.left  = m_rTitle.left + m_rTitle.Width() /20;
	m_rSysTimeText.right = m_rSysTimeText.left + 280;

	m_rSysTimeText.top  = m_rTitle.top + titleHeight / 3 - 1;
	m_rSysTimeText.bottom  = m_rSysTimeText.top + titleHeight / 3 + 2;


	m_rAwTipText.left  = rClient.left +800;
	m_rAwTipText.right = m_rAwTipText.left+ 300;
	m_rAwTipText.top  = m_rTitle.top + titleHeight /3 - 1;
	m_rAwTipText.bottom = m_rAwTipText.top + titleHeight /3 + 2;

	m_rHwTipText.left = m_rTitle.right  - 300;
	m_rHwTipText.right = m_rHwTipText.left  + 300;
	m_rHwTipText.top = m_rTitle.top + titleHeight / 3 - 1;
	m_rHwTipText.bottom = m_rHwTipText.top + titleHeight / 3 + 2;


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
END_MESSAGE_MAP()


// CColdEyeDlg 消息处理程序

BOOL CColdEyeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
		AfxMessageBox(_T("COM_KB 没有发现串口或串口被占用!"));
	}
	if (CSerialPort::GetInstance(COM_CAM)->InitPort(this, COM_CAM, 9600, 'N', 8, 2, EV_RXCHAR, 512))
	{
		CSerialPort::GetInstance(COM_CAM)->StartMonitoring();
	}
	else
	{
		AfxMessageBox(_T("COM_CAM 没有发现串口或串口被占用!"));
	}
	if (CExHardDrive::GetInstance()->ScanDisk(this))
	{
		CExHardDrive::GetInstance()->Updata();
	}


	CDBShadow::GetInstance()->BroadcaseInitFileMsg();

	m_AwTipText  = _T("自动看船开启");
	m_HwTipText  = _T("已关闭回家看船");


	SetTimer(TIMER_ID_SECOND_TICK, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CColdEyeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		//dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CPaintDC dc(this);
		CRect rClient;
		GetClientRect(rClient);

		dc.FillSolidRect(&rClient, RGB(171, 130, 255));

		CPen newPen(PS_SOLID, 1, RGB(255, 0, 0));

		CPen* pOldPen;
		CFont* pOldFont;
		pOldPen  = dc.SelectObject(&newPen);

		
		dc.Rectangle(m_rSysTimeText);
		dc.Rectangle(m_rAwTipText);
		dc.Rectangle(m_rHwTipText);


		dc.TextOutW(m_rSysTimeText.left + 1, m_rSysTimeText.top+1, m_SysTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		
		if (((CColdEyeApp*)AfxGetApp())->m_SysConfig.auto_watch_status) {
			dc.TextOutW(m_rAwTipText.left + 1, m_rAwTipText.top + 1, _T("自动看船已开启"));
		}
		else{
			dc.TextOutW(m_rAwTipText.left + 1, m_rAwTipText.top + 1, _T("自动看船已关闭"));
		}

		dc.TextOutW(m_rHwTipText.left + 1, m_rHwTipText.top + 1, _T("未开启回家看船"));

		dc.SelectObject(pOldPen);





		//CBitmap& bitmap = ((CColdEyeApp*)AfxGetApp())->m_Bitmap;
		//BITMAP bmp;
		//bitmap.GetBitmap(&bmp);

		//CDC mSrcDC;

		//mSrcDC.CreateCompatibleDC(&dc);

		//mSrcDC.SelectObject(bitmap);

		//dc.BitBlt(10, 50, bmp.bmWidth, bmp.bmHeight, &mSrcDC, 0, 0, SRCCOPY);



		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加专用代码和/或调用基类
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

	// TODO: 在此处添加消息处理程序代码
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
	if (port == COM_CAM)
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

		//这里判断CRC
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
				TRACE("控制灯泡\n");
				break;
			case 0x05:
				if (p->ch[4] == 1)
				{
					CCommunication::GetInstance()->RecAlarmProc(p->ch);
				}
				if (p->ch[4] == 2)
				{
					CCommunication::GetInstance()->RecOverAlarmProc(p->ch);
				}
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
		printf("U盘插入 %S\n", flag);
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
		printf("U盘拔出 %S\n", flag);
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//发送握手

	m_SysTime  = CTime::GetCurrentTime();
	Print("Second Tick");
	InvalidateRect(m_rSysTimeText);
	InvalidateRect(m_rAwTipText);
	InvalidateRect(m_rHwTipText);
	CDialogEx::OnTimer(nIDEvent);
}
