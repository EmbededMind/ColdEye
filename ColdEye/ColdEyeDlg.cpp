
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
#include "Com\Util.h"
#include "Database\DBShadow.h"
#include "Database\DBLogger.h"

#include "Pattern\MsgSquare.h"
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
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CColdEyeDlg 对话框



CColdEyeDlg::CColdEyeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLDEYE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MonitorPower = MonitorPowerOn;
	mBrightness = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness;
}



void CColdEyeDlg::UpdateLayout()
{
	CRect  rClient;
	GetClientRect(rClient);

	int title_height  = rClient.Height() /10;

	int firstBkRight  = rClient.Width() *3 /10;

	int thirdBkLeft   = rClient.Width() - rClient.Width() *3 /10;

	int secondBkLeft  = firstBkRight+1;
	int secondBkRight = thirdBkLeft-1;

	int offset  = title_height * 0.288675;

	m_pTitleBk_left[0].x  =  rClient.left;          m_pTitleBk_left[0].y  = 0;
	m_pTitleBk_left[1].x  =  firstBkRight + offset; m_pTitleBk_left[1].y = 0;
	m_pTitleBk_left[2].x  =  firstBkRight - offset; m_pTitleBk_left[2].y = title_height;
	m_pTitleBk_left[3].x  =  rClient.left;          m_pTitleBk_left[3].y = title_height;


	m_pTitleBk_center[0].x  = secondBkLeft + offset ;  m_pTitleBk_center[0].y = 0;
	m_pTitleBk_center[1].x  = secondBkRight + offset ; m_pTitleBk_center[1].y = 0;
	m_pTitleBk_center[2].x  = secondBkRight - offset ; m_pTitleBk_center[2].y = title_height;
	m_pTitleBk_center[3].x  = secondBkLeft  - offset ; m_pTitleBk_center[3].y = title_height;


	m_pTitleBk_right[0].x  = thirdBkLeft + offset;  m_pTitleBk_right[0].y  = 0;
	m_pTitleBk_right[1].x  = rClient.right;         m_pTitleBk_right[1].y  = 0;
	m_pTitleBk_right[2].x  = rClient.right;         m_pTitleBk_right[2].y  = title_height;
	m_pTitleBk_right[3].x  = thirdBkLeft - offset;  m_pTitleBk_right[3].y  = title_height;
	

	m_TextFont.DeleteObject();
	m_TextFont.CreatePointFont(title_height/4*10,  _T("方正兰亭中黑_GBK"));
	//m_TextFont.CreateFont(title_height / 4 * 10, _T("方正兰亭中黑_GBK"));
	LOGFONT  fontInfo;
	m_TextFont.GetLogFont(&fontInfo);


	m_rSysTimeText.left = rClient.left + offset;
	m_rSysTimeText.top  = rClient.top + title_height/4;
	m_rSysTimeText.right  = firstBkRight - offset;
	m_rSysTimeText.bottom = title_height - title_height/4;

	m_rAwTipText.left  = secondBkLeft + offset;
	m_rAwTipText.top  = m_rSysTimeText.top;
	m_rAwTipText.right  = secondBkRight -offset;
	m_rAwTipText.bottom = m_rSysTimeText.bottom;

	m_rHwTipText.left  = thirdBkLeft +offset;
	m_rHwTipText.top  = m_rSysTimeText.top;
	m_rHwTipText.right  = rClient.right - title_height /4;
	m_rHwTipText.bottom  = m_rSysTimeText.bottom;

	

	int flash_width  = m_rHwTipText.Width() * 1/5;
	int flash_height = flash_width*3/5;

	m_rHwTipText.right  -= flash_width;

	CRect rFlash;
	rFlash.left  = m_rHwTipText.right + 1;
	rFlash.right = rFlash.left + flash_width;
	rFlash.top   = (title_height- flash_height) /2;
	rFlash.bottom = rFlash.top + flash_height;

	GetDlgItem(IDC_UFLASH)->MoveWindow(rFlash);

	Print("Set wall pos (x:%d y:%d cx:%d cy:%d)", rClient.left, rClient.top+title_height, rClient.Width(), rClient.Height()-title_height);
	mWall.SetWindowPos(NULL, rClient.left, rClient.top + title_height, rClient.Width(), rClient.Height()- title_height, SWP_NOZORDER);


	CRect rectDlg;
	GetWindowRect(rectDlg);

	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);

	::MoveWindow(mMenu, (ScreenWidth- rectDlg.Width())/2, ScreenHeight / 10, ScreenHeight * 4 / 3, ScreenHeight-(ScreenHeight/10),true);
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
	ON_MESSAGE(USER_MSG_STOP_ALARM, &CColdEyeDlg::OnUserMsgStopAlarm)
	ON_MESSAGE(USER_MSG_ALARM_LIGHT, &CColdEyeDlg::OnUserMsgSetAlarmLight)
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


	
	int ScreenHeight  = GetSystemMetrics(SM_CYSCREEN);

	//LONG style  = GetWindowLong(m_hWnd, GWL_STYLE);
	////SetWindowLong(m_hWnd, GWL_STYLE,  style & (~(WS_CAPTION | WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME)));
	//SetWindowLong(m_hWnd, GWL_STYLE, style | WS_BORDER);
	DWORD dwStyle = GetStyle();//获取旧样式
	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	dwNewStyle &= dwStyle;//按位与将旧样式去掉
	SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);//设置成新的样式
	DWORD dwExStyle = GetExStyle();//获取旧扩展样式
	DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	dwNewExStyle &= dwExStyle;//按位与将旧扩展样式去掉
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewExStyle);//设置新的扩展样式
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！

	SetWindowPos(NULL, 0, 0, ScreenHeight*4/3, ScreenHeight, 0);

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
		GetDlgItem(IDC_UFLASH)->ShowWindow(true);
		CExHardDrive::GetInstance()->Updata();
	}


	CDBShadow::GetInstance()->BroadcaseInitFileMsg();

	m_AwTipText  = _T("自动看船开启");
	m_HwTipText  = _T("已关闭回家看船");


	SetTimer(TIMER_ID_SECOND_TICK, 1000, NULL);
	SetTimer(TIMER_ID_HANDLE, 10000, NULL);
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

		PaintTitle(&dc);

		CFont font;
		//font.CreatePointFont(240, _T("方正兰亭中黑_GBK"));

		CPen* pOldPen;
		CFont* pOldFont;

		CPen framePen;
		framePen.CreatePen(PS_SOLID, 2, RGB(135, 206, 235));

		LOGFONT fontInfo;
		m_TextFont.GetLogFont(&fontInfo);
	

		pOldFont = dc.SelectObject(&m_TextFont);
		pOldPen  = dc.SelectObject(&framePen);

		pOldPen  = dc.SelectObject(&framePen);

		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 255, 255));

		CString text  = m_SysTime.Format(_T("%Y-%m-%d %H:%M"));
		CSize textSize  = dc.GetTextExtent(text);

		int offset_x  = (m_rSysTimeText.Width() - textSize.cx) /2;
		int offset_y  = (m_rSysTimeText.Height() - textSize.cy) /2;

		dc.DrawTextW(text, m_rSysTimeText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);



		if (((CColdEyeApp*)AfxGetApp())->m_SysConfig.auto_watch_on) {
			text  = _T("自动看船已开启");
		}
		else {
			text  = _T("自动看船已关闭");
		}
		textSize = dc.GetTextExtent(text);
		offset_x = (m_rSysTimeText.Width() - textSize.cx) / 2;
		offset_y = (m_rSysTimeText.Height() - textSize.cy) / 2;
		dc.DrawTextW(text, m_rAwTipText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		//dc.Rectangle(m_rHwTipText);
		text  = _T("未开启回家看船");
		textSize = dc.GetTextExtent(text);
		offset_x = (m_rHwTipText.Width() - textSize.cx) / 2;
		offset_y = (m_rHwTipText.Height() - textSize.cy) / 2;
		dc.DrawTextW(text, m_rHwTipText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		dc.SelectObject(pOldFont);
		dc.SelectObject(pOldPen);

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
	CPort* pPort  = (CPort*)lParam;

	if (wParam) {
		Print("Search device succeed");
		PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_LOGIN, 0, (LPARAM)pPort);
	}
	else {
		Print("Search device failed");
		mSearchPort.push_back((CPort*)lParam);
	}

	return 0;
}


int CColdEyeDlg::SetVolumeLevel(int type)
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
			return (int)(10*fVolume);
		}
	}
	catch (...) {
		if (pAudioClient) pAudioClient->Release();
		if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
		if (pDevice) pDevice->Release();
		if (pDeviceEnumerator) pDeviceEnumerator->Release();
		throw;
	}
	return -1;
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
	int volume;
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
				{
					MSG msg;
					msg.wParam   = 666;
					msg.lParam  = USER_MSG_SYSTEM_CONFIG;

					CColdEyeApp* pApp  = (CColdEyeApp*)AfxGetApp();

					pApp->m_SysConfig.auto_watch_on = pApp->m_SysConfig.auto_watch_on>0?0:1;

					CDBLogger::GetInstance()->LogAutoWatch(m_SysTime, pApp->m_SysConfig.auto_watch_on);

					CMsgSquare::GetInstance()->Broadcast(msg);
				}
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
				volume = this->SetVolumeLevel(ENLARGE_VOLUME);
				::PostMessage(mMenu, USER_MSG_SYS_VOLUM, volume, NULL);
				break;
			case KB_DOWN:
				keybd_event(VK_DOWN, 0, 0, 0);
				keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_BRIUP:
				mBrightness++;
				if (mBrightness > 10)
					mBrightness = 10;
				mGammaRamp.SetBrightness(NULL,50 + mBrightness*15);
				//PostMessage();
				break;
			case KB_FUNC:
				keybd_event(VK_F8, 0, 0, 0);
				keybd_event(VK_F8, 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_VOLDOWN:
				volume = this->SetVolumeLevel(REDUCE_VOLUME);
				::PostMessage(mMenu,USER_MSG_SYS_VOLUM,volume,NULL);
				break;
			case KB_TALKQUIET:
				SetVolumeLevel(1);
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('S', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_BRIDOWN:
				mBrightness--;
				if (mBrightness < 0)
					mBrightness = 0;
				Print("brightness %d", mBrightness);
				mGammaRamp.SetBrightness(NULL, mBrightness * 25);
				//PostMessage();
				break;
			case KB_SWITCH:
			{
				//char str[120] = "shutdown -f -s -t 0";
				//system(str);
				//break;
				if (m_MonitorPower != MonitorPowerOn)
				{
					m_MonitorPower = MonitorPowerOn;
				}
				else if(m_MonitorPower != MonitorPowerOff)
				{
					m_MonitorPower = MonitorPowerOff;
				}
				::PostMessage(HWND_BROADCAST,
					WM_SYSCOMMAND,
					SC_MONITORPOWER,
					(LPARAM)m_MonitorPower);
				break;
			}
			case KB_PTTDOWN:
				SetVolumeLevel(0);
				keybd_event(VK_CONTROL, 0, 0, 0);
				keybd_event('T', 0, 0, 0);
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event('T', 0, KEYEVENTF_KEYUP, 0);
				break;
			case KB_PTTUP:
				SetVolumeLevel(1);
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

		//printf("COM_CAMERA message NO.%d : ", CAmessage_NO);
		//CAmessage_NO++;
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

		//if (p->ch[1] == 0x02 && p->ch[2] == 0x01)
		uint64_t mac64 = CUtil::ArrayToUint64(&p->ch[6]);
		CCamera *pDev;
		if (Mac_CCamera_Map.find(mac64) == Mac_CCamera_Map.end())
		{
			Print("pDev == NULL");
			pDev = NULL;
		}
		else
		{
			Print("pDev != NULL");
			pDev = Mac_CCamera_Map[mac64];
		}
		
		if (true)
		{
			switch (p->ch[3])
			{
			case 0x01:
				if(p->ch[5] == 1)
					CCommunication::GetInstance()->ReplySetVolume(pDev);
				else
					CCommunication::GetInstance()->ReplySetVolume(0);
				break;
			case 0x02:
				Print("case 0x02");
				switch (p->ch[4])
				{
				case 1:
					Print("p->ch[4] == 1");
					if (p->ch[5] == 1)
						CCommunication::GetInstance()->ReplyHostTalk(pDev);
					else
						CCommunication::GetInstance()->ReplyHostTalk(0);
					break;
				case 2:
					Print("p->ch[4] == 2");
					if (p->ch[5] == 1)
						CCommunication::GetInstance()->ReplyCameraTalk(pDev);
					else
						CCommunication::GetInstance()->ReplyCameraTalk(0);
					break;
				case 3:
					Print("p->ch[4] == 3");
					CCommunication::GetInstance()->ReplyStopTalk();
					break;
				default:
					break;
				}
				break;
			case 0x03:
				CCommunication::GetInstance()->ReplyCameraAskTalk(pDev);
				break;
			case 0x04:
				if (p->ch[4] == 1)
				{
					if(p->ch[5] == 1)
						CCommunication::GetInstance()->ReplyAlarm(pDev);
					else
						CCommunication::GetInstance()->ReplyAlarm(0);
				}
				if (p->ch[4] == 2)
				{
					if (p->ch[5] == 1)
						CCommunication::GetInstance()->ReplyStopAlarm(pDev);
					else
						CCommunication::GetInstance()->ReplyStopAlarm(0);
				}
				break;
			case 0x05:
				/*CCommunication::GetInstance()->RecHandleProc(p->ch);*/
				if (p->ch[4] == 1)  //端口状态信息。
				{
					CCommunication::GetInstance()->ReplyHandle(1);
					CPortManager*  pPortMgr  = CPortManager::GetInstance();
					CPort*         pPort ;
					for (int i = 0; i < 6; i++) {
						pPort  = &pPortMgr->mPorts[i];

						bool isOnline    = (p->ch[6+i] & 0x80) > 0;
						bool isReplaced  = (p->ch[6+i] & 0x10) > 0;

						if (pPort->m_State == OFFLINE) {  
						    // 检测到插入
							if (isOnline) {
								Print("---->Plug event");
								pPort->m_State  = PENDING_MAC;
								mPendMacPort.push_back(i+1); // 加入请求mac的队列
							}
						}
						else if (pPort->m_State == ONLINE) {
							//检测到拔出
							
							if (!isOnline) {
								Print("Pull event<----");
								pPort->m_State  = OFFLINE;
							}
							//检测到更换
							else if (isReplaced) {
								Print("Replace event---");
								pPort->m_State  = PENDING_MAC;
								mPendMacPort.push_back(i+1);// 加入请求mac的队列
							}
						}


						//PortOption resOpt  = pPortMgr->mPorts[i].ParseState((p->ch[6+i] & 0x80) > 0, (p->ch[6+i] & 0x10) > 0);
						//
						//if (resOpt == PEND_MAC) {
						//	//发送请求mac的串口命令
						//	Print("%d pend mac and state is:%d", i+1, pPortMgr->mPorts[i].m_State);
						//	mPendMacPort.push_back(i+1);
						//}
					}
				}
				else if (p->ch[4] == 2) //端口mac
				{
					CCommunication::GetInstance()->ReplyGetPortMac(1);
					//for (int i = 0; i < p->num; i++) {
					//	printf("%02X ", p->ch[i]);
					//}
					//printf("\n");
					// p->ch[6]  mac
					// p->ch[5]  id
					if (p->ch[5] > 0 && p->ch[5] <= 6) {					
						CPortManager* pPortMgr  = CPortManager::GetInstance();
						CPort* pPort  = &(pPortMgr->mPorts[p->ch[5]-1]);
						
						// 此端口没有id，说明是第一次被插（... 处女？），则分配一个1～6的id给她。
						if (pPort->GetId() == 0) {
							int cnt  = 0;
							for (int i = 0; i < 6; i++) {
								if (pPortMgr->mPorts[i].GetId() > 0) {
									cnt++;
								}
							}

							if (cnt < 6) {
								pPortMgr->BindPortId(pPort, cnt+1);
							}
							else {
								Print("What happened!");
							}
						}


						if (pPort->m_State == PENDING_MAC) {
							CUtil::MacNumberToStr( &(p->ch[6]), pPort->GetMac());

							// 此端口未绑定摄像头
							if (pPort->m_pCamera == NULL) {
							Print("Unbinded port");
								pPort->m_State  = PENDING_CAMERA;								
								PostThreadMessage(((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, (LPARAM)pPort);
							}
							// 此端口已绑定摄像头
							else {
								// 请求的mac和已绑定的mac相同，说明插入的设备和此前拔出的是同一个。
								if (strcmp(pPort->m_pCamera->mCommonNetConfig.sMac, pPort->GetMac()) == 0) {
									//，重新登录
									pPort->m_State  = ONLINE;
									Print("Pend mac equal binded mac, so wait for reconnect");
								}
								// 不相同，需要将原来绑定的摄像头注销并删除对应Surface，然后请求新的摄像头。
								else {
									// 注销，删除
									Print("Need logoff and delete");
									::SendMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_LOGOFF, 2, (LPARAM)pPort);

									pPort->m_State  = PENDING_CAMERA;
									PostThreadMessage(((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, (LPARAM)pPort);
								}	
							}
					
						}
						else {
							Print("Rec mac but not pending mac");
						}
					}
					break;
				}
				case 6:
					CCommunication::GetInstance()->ReplyControlLED(1);
					break;
				case 7:
					CCommunication::GetInstance()->ReplySetLED(1);
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
		GetDlgItem(IDC_UFLASH)->ShowWindow(true);
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
		GetDlgItem(IDC_UFLASH)->ShowWindow(false);
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//发送握手
	static int i = 0;
	static int CntDiv_10S  = 0;
	if (nIDEvent == TIMER_ID_SECOND_TICK)
	{
		CntDiv_10S++;
		if (CntDiv_10S >= 10) {
			CntDiv_10S = 0;

			if (mSearchPort.size() > 0) {
				CPort* pPort = mSearchPort.front();
				mSearchPort.pop_front();
				PostThreadMessage(((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, (LPARAM)pPort);
			}
		}

		m_SysTime = CTime::GetCurrentTime();

		CDBLogger::GetInstance()->LogSystemTime(m_SysTime);


		InvalidateRect(m_rSysTimeText);
		InvalidateRect(m_rAwTipText);
		InvalidateRect(m_rHwTipText);

	}
	else if (nIDEvent == TIMER_ID_HANDLE)
	{
		if (mPendMacPort.size() > 0) {
			uint8_t id = mPendMacPort.front();
			mPendMacPort.pop_front();

			Print("%d Pend mac", id);

			CCommunication::GetInstance()->GetPortMac(id);

		}
		else {
			CCommunication::GetInstance()->Handle();		}
	}
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
			// 向 m_Id 号端口发送 设置 音量命令。
			Print("Set %d camera vol:%d", pPort->m_Id, pConfig->Volumn);
			CCommunication::GetInstance()->SetVolume(pPort->GetCamera(), pConfig->Volumn);
		}
	}

	return 0;
}

LRESULT CColdEyeDlg::OnUserMsgStopAlarm(WPARAM wParam, LPARAM lParam)
{
Print("MSG Stop Alarm");
	CCamera *pDev = (CCamera*)lParam;
	CCommunication::GetInstance()->StopAlarm(pDev);
	return 0;
}

LRESULT CColdEyeDlg::OnUserMsgSetAlarmLight(WPARAM wParam, LPARAM lParam)
{
	CCommunication::GetInstance()->SetLED(wParam);
	return LRESULT(0);
}
