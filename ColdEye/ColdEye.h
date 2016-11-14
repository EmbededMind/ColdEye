
// ColdEye.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


#include "Wnd\MainWnd.h"
#include "Wnd\WallWnd.h"
#include "Wnd\MyMenuWnd.h"

#include "WallDlg.h"

// CColdEyeApp: 
// 有关此类的实现，请参阅 ColdEye.cpp
//

class CColdEyeApp : public CWinApp
{
public:
	CColdEyeApp();

// 重写
public:
	virtual BOOL InitInstance();

	HWND      GetMainWndHandle();
	HWND      GetWallWndHandle();
	CWallWnd* GetWallWnd();
	HWND      GetMenuWndHandle();
	UINT      GetLoginThreadPID();

	void      SetWallWnd(CWallWnd* pWallWnd);
	void      SetMenuWnd(CMyMenuWnd* pMenuWnd);

	void      SetWallDlg(CWallDlg* pWallDlg);
	CWallDlg* GetWallDlg();



private:
	CMainWnd* _m_pMainWnd;
	CWallWnd* m_pWallWnd;
	CMyMenuWnd* m_pMenuWnd;

	CWallDlg*  m_pWallDlg;

	HANDLE m_hLoginThread;
	UINT   m_LoginPID;

	void CheckFileDirectory();
	void CheckDatabase();


// 实现

	DECLARE_MESSAGE_MAP()
};

extern CColdEyeApp theApp;