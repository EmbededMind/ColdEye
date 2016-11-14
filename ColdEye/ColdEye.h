
// ColdEye.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "Wnd\MyMenuWnd.h"

#include "WallDlg.h"

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



	HWND      GetMenuWndHandle();
	UINT      GetLoginThreadPID();

	void      SetMenuWnd(CMyMenuWnd* pMenuWnd);

	void      SetWallDlg(CWallDlg* pWallDlg);
	CWallDlg* GetWallDlg();



private:
	CMyMenuWnd* m_pMenuWnd;

	CWallDlg*  m_pWallDlg;

	HANDLE m_hLoginThread;
	UINT   m_LoginPID;

	void CheckFileDirectory();
	void CheckDatabase();


// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CColdEyeApp theApp;