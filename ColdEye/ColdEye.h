
// ColdEye.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


#include "Wnd\MainWnd.h"
#include "Wnd\WallWnd.h"
#include "Wnd\MyMenuWnd.h"

// CColdEyeApp: 
// �йش����ʵ�֣������ ColdEye.cpp
//

class CColdEyeApp : public CWinApp
{
public:
	CColdEyeApp();

// ��д
public:
	virtual BOOL InitInstance();

	HWND      GetMainWndHandle();
	HWND      GetWallWndHandle();
	CWallWnd* GetWallWnd();
	HWND      GetMenuWndHandle();
	UINT      GetLoginThreadPID();

	void      SetWallWnd(CWallWnd* pWallWnd);
	void      SetMenuWnd(CMyMenuWnd* pMenuWnd);


private:
	CMainWnd* _m_pMainWnd;
	CWallWnd* m_pWallWnd;
	CMyMenuWnd* m_pMenuWnd;


	HANDLE m_hLoginThread;
	UINT   m_LoginPID;

	void CheckFileDirectory();
	void CheckDatabase();


// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CColdEyeApp theApp;