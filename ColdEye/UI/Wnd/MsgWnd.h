#pragma once

#include "ColdEyeDlg.h"
#include "ColdEye.h"
#include "Wnd\MsgWnd.h"

#include "UIlib.h"
using namespace DuiLib;


#define MSGID_CANCEL	0
#define MSGID_OK		1
#define MSGID_RECORD	2

#define TIME_RECORD_VOICE	1
#define TIME_PLAY_VOICE		3

class CColdEyeDlg;

class CMsgWnd : public WindowImplBase
{

public:
	CMsgWnd();
	~CMsgWnd();

	void SetMsg(LPCTSTR text1, LPCTSTR text2);
	void SetTitle(LPCTSTR lpstrTitle);

	
public:
	static int MessageBox(HWND hParent, LPCTSTR skinType, LPCTSTR text1, LPCTSTR text2,LPARAM lparam,WPARAM wParam)
	{
		CMsgWnd* pWnd = new CMsgWnd();
		//((CColdEyeDlg*)((CColdEyeApp*)AfxGetApp())->m_pMainWnd)->mMessageBox = pWnd;
		pWnd->videoType = (UINT8)wParam;

		pWnd->SkinType = skinType;
		pWnd->pRecordInfo = (list<CRecordFileInfo*>*)lparam;
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetMsg(text1,text2);
		return pWnd->ShowModal();
	}

	static void ShowMessageBox(HWND hParent, LPCTSTR skinType, LPCTSTR text1, LPCTSTR text2)
	{
		CMsgWnd* pWnd = new CMsgWnd();
		pWnd->SkinType = skinType;
		pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0);
		pWnd->CenterWindow();
		pWnd->SetMsg(text1, text2);
		pWnd->ShowWindow(true);
	}

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void RecordVoice();
	void AlarmVoicePlay();
public:
	CDuiString SkinType;
	CButtonUI *pButton_ok;
	CButtonUI *pButton_cancel;
	CButtonUI *pButton_record;
	CColdEyeDlg *pMainDlg;
	UINT8 _time;
	list<CRecordFileInfo*>* pRecordInfo;
	ULONGLONG totalSize;
	ULONGLONG sendedSize;
	UINT8 videoType;
};

