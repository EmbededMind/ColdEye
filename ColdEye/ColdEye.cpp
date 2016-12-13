// ColdEye.cpp : ����Ӧ�ó��������Ϊ��


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


// CColdEyeApp ����

CColdEyeApp::CColdEyeApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CColdEyeApp ����

CColdEyeApp theApp;


// CColdEyeApp ��ʼ��

BOOL CColdEyeApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

#ifdef USE_PRINT
	InitConsoleWindow();
#endif
	

	m_Bitmap.LoadBitmap(IDB_NSLIENT);

	CheckFileDirectory();

	// �����ݿ�
	bool bRet = sqlite.Open("cold_eye.db");
	if (bRet == false) {
		Print("Database open failed");
		ASSERT(FALSE);
	}


	//  ��������������������ʧ����������ΪĬ��ֵ��
	if (!LoadSystemConfig()) {
		MakeSystemConfigDefault();
	}

	CDBShadow::GetInstance()->SynchronizeWithDB();

	CTime time  = CTime::GetCurrentTime();

	CDBLogger* pLogger  = CDBLogger::GetInstance();

	pLogger->GenerateLastPowerOffLog();

	pLogger->LogPowerOn(time, 1);

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

	//RecordFileMetabolism attach�۲���
	CRecordFileMetabolism::GetInstance()->Attach(CDBShadow::GetInstance());


	CColdEyeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.SetFocus();
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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
		m_SysConfig.alarm_sound_sec  =           stmt->ValueInt(9);
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


	sprintf_s(sqlStmt, "INSERT INTO host_config VALUES('%s', %d, %d, %d,%d, %d, %d, %d, %d,%d);",
		m_SysConfig.boat_name.c_str(), m_SysConfig.auto_watch_on,m_SysConfig.watch_time_begining, m_SysConfig.watch_time_end,
		m_SysConfig.alarm_light_onoff, m_SysConfig.alarm_sound_onoff, m_SysConfig.alarm_sound_id, m_SysConfig.brightness, m_SysConfig.volumn, m_SysConfig.alarm_sound_sec);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}



void CColdEyeApp::StoreAlarmLightConfig()
{
	char sqlStmt[128];
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
	char sqlStmt[128];
	sprintf_s(sqlStmt, "UPDATE host_config SET arm_snd_on = %d, arm_snd_id = %d, arm_snd_sec = %d;",m_SysConfig.alarm_sound_onoff, m_SysConfig.alarm_sound_id, m_SysConfig.alarm_sound_sec);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		Print("Sql done:%s", sqlStmt);
	}
}


void CColdEyeApp::StoreSystemConfig()
{
	char sqlStmt[128];
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
	char sqlStmt[128];
	sprintf_s(sqlStmt, "UPDATE host_config SET aw_begining = %d, aw_end = %d;", tBegining, tEnd);

	if (sqlite.DirectStatement(sqlStmt)) {
		m_SysConfig.watch_time_begining  = tBegining;
		m_SysConfig.watch_time_end  = tEnd;
		Print("Sql done:%s", sqlStmt);
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
						pCamera->SetCameraTime(CTime::GetCurrentTime());
						pCamera->GetSDKCameraParam();
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