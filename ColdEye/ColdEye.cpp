// ColdEye.cpp : ����Ӧ�ó��������Ϊ��


#include "stdafx.h"
#include "ColdEye.h"
#include "ColdEyeDlg.h"

#include "netsdk.h"


#include "Device\Camera.h"

#include "UIlib.h"


#include "Control\AlarmLightUI.h"
#include "Control\AlarmVoiceListUI.h"
#include "Control\AlarmVoiceSwitchUI.h"
#include "Control\CameraSwitchUI.h"
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
	//


	CheckFileDirectory();
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
	REGIST_DUICONTROL(CCameraSwitchUI);
	REGIST_DUICONTROL(CMyLabelUI);
	REGIST_DUICONTROL(CMySliderUI);
	REGIST_DUICONTROL(CAlarmVoiceSwitchUI);
	REGIST_DUICONTROL(CAlarmVoiceListUI);
	REGIST_DUICONTROL(CAlarmLightUI);
	REGIST_DUICONTROL(CMyListUI);

	//_m_pMainWnd = new CMainWnd();
	//_m_pMainWnd->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//_m_pMainWnd->CenterWindow();
	//_m_pMainWnd->ShowModal();



	CColdEyeDlg dlg;
	m_pMainWnd = &dlg;
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
	if (!PathIsDirectory(_T(RECORD_PATH)))
	{
		CreateDirectory(_T(RECORD_PATH), NULL);
	}
	if (!PathIsDirectory(_T(NORMAL_RECORD_PATH)))
	{
		CreateDirectory(_T(NORMAL_RECORD_PATH), NULL);
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
	static SDK_CONFIG_NET_COMMON_V2 DeviceNetCommon[10];
	int iRetLength;

	while (GetMessage(&msg, 0, 0, 0))
	{
		switch (msg.message)
		{
		     case USER_MSG_SCAN_DEV:
				 {
					 BOOL bRet = H264_DVR_SearchDevice((char*)DeviceNetCommon, sizeof(SDK_CONFIG_NET_COMMON_V2) * 10, &iRetLength, 7000);
					 if (bRet)
					 {
						 int iDevNumber = iRetLength / sizeof(SDK_CONFIG_NET_COMMON_V2);
						 if (iDevNumber > 0)
						 {
							 PostMessage( AfxGetApp()->m_pMainWnd->m_hWnd, USER_MSG_SCAN_DEV, iDevNumber, (LPARAM)DeviceNetCommon);
						 }
					 }
					 else {
						 TRACE("Search nothing\n");
					 }
				 }
				 break;
			//-----------------------------------------------------------------------------------
			 case USER_MSG_LOGIN:
		    	 {
					CCamera* pCamera = (CCamera*)msg.lParam;
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
					 CCamera* pCamera = (CCamera*)msg.lParam;

					 if (pCamera->Login()) {
						 PostMessage((HWND)msg.wParam, USER_MSG_RELOGIN, true, msg.lParam);
					 }
				 }
				 break;
			//-----------------------------------------------------------------------------------
		}
		DispatchMessage(&msg);
	}
	

	return 0;
}