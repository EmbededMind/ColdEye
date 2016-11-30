// ColdEye.cpp : 定义应用程序的类行为。


#include "stdafx.h"
#include "ColdEye.h"
#include "ColdEyeDlg.h"

#include "netsdk.h"


#include "Device\Camera.h"
#include "Device\Port.h"

#include "Database\DBShadow.h"
#include "Database\DBLogger.h"
#include "Device\PortManager.h"

#include "UIlib.h"


#include "Control\AlarmVoiceListUI.h"
#include "Control\SwitchUI.h"
#include "Control\KeyBoardUI.h"
#include "Control\MenuItemUI.h"
#include "Control\MyEditUI.h"
#include "Control\MyLabelUI.h"
#include "Control\MyListUI.h"
#include "Control\MySliderUI.h"
#include "Control\PopupMenuUI.h"
#include "Control\ShipNameItemUI.h"
#include "Control\TimeButtonUI.h"
#include "Control\VideoListUI.h"
#include "Control\RecordvoiceUI.h"
#include "Control\RevordVoiceBtUI.h"

#include "File\RecordFileMetabolism.h"

#ifdef USE_PRINT
#include <io.h>
#include <fcntl.h>
#include <stdio.h>


void InitConsoleWindow()
{
	::AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
}
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 
UINT __stdcall LoginThread(PVOID pM);


// CColdEyeApp

BEGIN_MESSAGE_MAP(CColdEyeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CColdEyeApp 构造

CColdEyeApp::CColdEyeApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CColdEyeApp 对象

CColdEyeApp theApp;


// CColdEyeApp 初始化

BOOL CColdEyeApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

#ifdef USE_PRINT
	InitConsoleWindow();
#endif
	

	m_Bitmap.LoadBitmap(IDB_NSLIENT);

	CheckFileDirectory();

	// 打开数据库
	bool bRet = sqlite.Open("cold_eye.db");
	if (bRet == false) {
		Print("Database open failed");
		ASSERT(FALSE);
	}


	//  载入主机设置项，如果载入失败设置项设为默认值。
	if (!LoadSystemConfig()) {
		MakeSystemConfigDefault();
	}

	CDBShadow::GetInstance()->SynchronizeWithDB();

	CDBLogger::GetInstance()->LogPowerOn(CTime::GetCurrentTime(), 1);
	CDBLogger::GetInstance()->GenerateLastPowerOffLog();

	CPortManager::GetInstance()->LoadPortsParam();

	// Start login thread
	m_hLoginThread = (HANDLE)_beginthreadex(NULL, 0, LoginThread, NULL, 0, &m_LoginPID);



	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	
	CPaintManagerUI::SetResourcePath(_T(UI_RESOURCE_PATH));

	using namespace DuiLib;

	REGIST_DUICONTROL(CPopupMenuUI);
	REGIST_DUICONTROL(CVideoListUI);
	REGIST_DUICONTROL(CTimeButtonUI);
	REGIST_DUICONTROL(CMenuItemUI);
	REGIST_DUICONTROL(CMyEditUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CShipNameItemUI);
	REGIST_DUICONTROL(CMyLabelUI);
	REGIST_DUICONTROL(CMySliderUI);
	REGIST_DUICONTROL(CAlarmVoiceListUI);
	REGIST_DUICONTROL(CSwitchUI);
	REGIST_DUICONTROL(CMyListUI);
	REGIST_DUICONTROL(CRecordvoiceUI);
	REGIST_DUICONTROL(CRevordVoiceBtUI);

	//_m_pMainWnd = new CMainWnd();
	//_m_pMainWnd->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//_m_pMainWnd->CenterWindow();
	//_m_pMainWnd->ShowModal();

	//RecordFileMetabolism attach观察者
	CRecordFileMetabolism::GetInstance()->Attach(CDBShadow::GetInstance());


	CColdEyeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

bool CColdEyeApp::LoadSystemConfig()
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "SELECT * FROM host_config;");

	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);

	if (stmt->NextRow()) {
		m_SysConfig.boat_name =                  stmt->ValueString(0);
		m_SysConfig.auto_watch_on =              stmt->ValueInt(1);
		m_SysConfig.watch_time_begining =        stmt->ValueInt(2);
		m_SysConfig.watch_time_end =             stmt->ValueInt(3);
		m_SysConfig.alarm_light_onoff  =         stmt->ValueInt(4);
		m_SysConfig.alarm_sound_onoff =          stmt->ValueInt(5);
		m_SysConfig.alarm_sound_id =             stmt->ValueInt(6);
		m_SysConfig.brightness =                 stmt->ValueInt(7);
		m_SysConfig.volumn =                     stmt->ValueInt(8);
		return true;
	}
	else {
		return false;
	}
}





void CColdEyeApp::MakeSystemConfigDefault()
{
	char sqlStmt[128];

	m_SysConfig.boat_name = "SealedGhost";

	m_SysConfig.auto_watch_on = 1;
	m_SysConfig.watch_time_begining = 18 * 60;
	m_SysConfig.watch_time_end = 12 * 60;

	m_SysConfig.alarm_light_onoff = 0;

	m_SysConfig.alarm_sound_onoff  = 1;
	m_SysConfig.alarm_sound_id = 0;

	m_SysConfig.brightness = 0;
	m_SysConfig.volumn = 5;


	sprintf_s(sqlStmt, "INSERT INTO host_config VALUES('%s', %d, %d, %d,%d, %d, %d, %d, %d);",
		m_SysConfig.boat_name.c_str(), m_SysConfig.auto_watch_on,m_SysConfig.watch_time_begining, m_SysConfig.watch_time_end,
		m_SysConfig.alarm_light_onoff, m_SysConfig.alarm_sound_onoff, m_SysConfig.alarm_sound_id, m_SysConfig.brightness, m_SysConfig.volumn);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}



void CColdEyeApp::StoreAlarmLightConfig()
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "UPDATE host_config SET arm_lgt_on = %d;", m_SysConfig.alarm_light_onoff);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		Print("Sql done:%s", sqlStmt);
	}
}


void CColdEyeApp::StoreAlarmSoundConfig()
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "UPDATE host_config SET arm_snd_on = %d, arm_snd_id = %d;",m_SysConfig.alarm_sound_onoff, m_SysConfig.alarm_sound_id);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		Print("Sql done:%s", sqlStmt);
	}
}


void CColdEyeApp::StoreSystemConfig()
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "UPDATE host_config SET brtness = %d, vol = %d;", m_SysConfig.brightness, m_SysConfig.volumn);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		Print("Sql done:%s", sqlStmt);
	}
}



void CColdEyeApp::StoreBoatName()
{
	char sqlStmt[128];
	sprintf_s(sqlStmt, "UPDATE host_config SET boat_name = '%s';", m_SysConfig.boat_name.c_str());
	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		Print("Sql done:%s", sqlStmt);
	}
}


bool CColdEyeApp::SetAwTime(DWORD tBegining, DWORD tEnd)
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "UPDATE host_config SET aw_begining = %d, aw_end = %d;", tBegining, tEnd);

	if (sqlite.DirectStatement(sqlStmt)) {
		m_SysConfig.watch_time_begining  = tBegining;
		m_SysConfig.watch_time_end  = tEnd;
		return true;
	}
	else {
		Print("Sql error:%s", sqlStmt);
		return false;
	}
}



HWND CColdEyeApp::GetMenuWndHandle()
{
	return 0;
}



UINT CColdEyeApp::GetLoginThreadPID()
{
	return m_LoginPID;
}






void CColdEyeApp::SetMenuWnd(CMyMenuWnd* pMenuWnd)
{
	this->m_pMenuWnd = pMenuWnd;
}


void CColdEyeApp::SetWallDlg(CWallDlg* pWall)
{
	this->m_pWallDlg = pWall;
}


CWallDlg* CColdEyeApp::GetWallDlg()
{
	return this->m_pWallDlg;
}


void CColdEyeApp::CheckFileDirectory()
{
	if (!PathIsDirectory(_T(NORMAL_RECORD_PATH)))
	{
		CreateDirectory(_T(NORMAL_RECORD_PATH), NULL);
	}
	if (!PathIsDirectory(_T(RECORD_VOICE_PATH)))
	{
		CreateDirectory(_T(RECORD_VOICE_PATH), NULL);
	}
	if (!PathIsDirectory(_T(ALARM_RECORD_PATH)))
	{
		CreateDirectory(_T(ALARM_RECORD_PATH), NULL);
	}

	if (!PathIsDirectory(_T(SOUND_PATH)))
	{
		CreateDirectory(_T(SOUND_PATH), NULL);
	}

	CString strRecordPath, temp;
	for (int i = 1; i <= 6; i++)
	{
		temp.Format(_T("%d\\"), i);
		strRecordPath = _T(NORMAL_RECORD_PATH) + temp;
		if (!PathIsDirectory(strRecordPath))
		{
			CreateDirectory(strRecordPath, NULL);
		}
		strRecordPath = _T(ALARM_RECORD_PATH) + temp;
		if (!PathIsDirectory(strRecordPath))
		{
			CreateDirectory(strRecordPath, NULL);
		}
	}
	
}

void CColdEyeApp::CheckDatabase()
{

}




UINT __stdcall LoginThread(PVOID pM)
{
	MSG msg;
	SDK_CONFIG_NET_COMMON_V2 DeviceNetCommon[10];
	static int ScanLength  = 0;
	int iRetLength;

	while (GetMessage(&msg, 0, 0, 0))
	{
		switch (msg.message)
		{
		     case USER_MSG_SCAN_DEV:
				 {
					bool hasFound  = false;
					 CPort* pPort  = (CPort*)msg.lParam;

					 Print("searching mac %s", pPort->GetMac());

					 int i  = 0;
					 
					 for (; i < ScanLength; i++) {
						Print("Scan mac:%s", DeviceNetCommon[i].sMac);
						 if (strcmp(DeviceNetCommon[i].sMac, pPort->GetMac()) == 0) {
							Print("Find mac:%s", DeviceNetCommon[i].sMac);
							hasFound  = true;
							break;
						 }
					 }

					 if (!hasFound) {
						 BOOL bRet = H264_DVR_SearchDevice((char*)DeviceNetCommon, sizeof(SDK_CONFIG_NET_COMMON_V2) * 10, &iRetLength, 7000);
						 if (bRet)
						 {
							 int ScanLength = iRetLength / sizeof(SDK_CONFIG_NET_COMMON_V2);

							 for (i = 0; i < ScanLength; i++) {
								 Print("Scan mac:%s", DeviceNetCommon[i].sMac);
								 if (strcmp(DeviceNetCommon[i].sMac, pPort->GetMac()) == 0) {
									 Print("Find mac:%s", DeviceNetCommon[i].sMac);
									 hasFound = true;									 
									 break;
								 }
							 }
						 }							
					 }

					 if (hasFound) {
					    
						 //if (strcmp(DeviceNetCommon[i].sMac, pPort->GetMac()) != 0) {

						 //}

						 CCamera* pCamera = new CCamera();


						 pCamera->SetCommonNetConfig(&DeviceNetCommon[i]);

						 pPort->m_pCamera = pCamera;
						 PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, USER_MSG_SCAN_DEV, true, msg.lParam);
					 }
					 else {
						 PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, USER_MSG_SCAN_DEV, false, msg.lParam);
					 }
				 }
				 break;
			//-----------------------------------------------------------------------------------
			 case USER_MSG_LOGIN:
		    	 {
					CPort* pPort  = (CPort*)msg.lParam;
					CCamera* pCamera = pPort->m_pCamera;
					
					CWallDlg* pWallDlg = ((CColdEyeApp*)AfxGetApp())->GetWallDlg();
					ASSERT(pWallDlg != nullptr);

					if (pCamera->Login())
					{
						PostMessage(pWallDlg->m_hWnd, USER_MSG_LOGIN, true, msg.lParam);
					}
					else
					{				
						PostMessage(pWallDlg->m_hWnd, USER_MSG_LOGIN, false, msg.lParam);
					}
			     }
				 break;
			//----------------------------------------------------------------------------
			 case USER_MSG_RELOGIN:
				 {
					 CCamera* pCamera = ((CPort*)msg.lParam)->m_pCamera;

					 Print("Relogining...");
					 if (pCamera->Login()) {
						 PostMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_RELOGIN, true, msg.lParam);
					 }
					 else {
						 PostMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_RELOGIN, false, msg.lParam);
					 }
				 }
				 break;
			//-----------------------------------------------------------------------------------
			 case USER_MSG_CAMERA_PARAM:
				 {
					 CCamera* pCamera = (CCamera*)msg.lParam;
					 if (msg.wParam) {
						 pCamera->SetSDKCameraParam() ;
					 }
					 else {
						 pCamera->GetSDKCameraParam();
					 }
				 }
				 break;
		}
		DispatchMessage(&msg);
	}
	

	return 0;
}