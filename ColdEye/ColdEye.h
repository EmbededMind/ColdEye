
// ColdEye.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "Wnd\MyMenuWnd.h"

#include "WallDlg.h"



typedef struct {
	std::string boat_name;
	DWORD       watch_time_begining;
	DWORD       watch_time_end;
	bool        auto_watch_status;
	uint16_t    alarm_sound;
	uint16_t    brightness;
}HostConfig;


// CColdEyeApp: 
// 有关此类的实现，请参阅 ColdEye.cpp
//

class CColdEyeApp : public CWinApp
{
public:
	CColdEyeApp();

// 重写
public:
	CBitmap m_Bitmap;

	virtual BOOL InitInstance();

	HostConfig   m_SysConfig;
	bool      LoadSystemConfig();
	//bool      SetSystemConfig(CString& boat_name);
	//bool      SetSystemConfig(uint16_t  brightness, uint16_t volumn);
	void      MakeSystemConfigDefault();
	bool      SetAwTime(DWORD tBegining, DWORD tEnd);


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


// 实现

	DECLARE_MESSAGE_MAP()
};

extern CColdEyeApp theApp;