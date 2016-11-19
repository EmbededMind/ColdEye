#pragma once

#include "UIlib.h"
using namespace DuiLib;


#define MSGID_CANCEL	0
#define MSGID_OK		1
#define MSGID_RECORD	2

class CMsgWnd : public WindowImplBase
{

public:
	CMsgWnd();
	~CMsgWnd();

	//struct textInfo {
	//	LPCTSTR text;
	//	LPCTSTR	textcolor;
	//	CRect	pos;
	//};

	void SetMsg(LPCTSTR text1, LPCTSTR text2);
	void SetTitle(LPCTSTR lpstrTitle);

	
public:
	static int MessageBox(HWND hParent, LPCTSTR skinType, LPCTSTR text1, LPCTSTR text2)
	{
		CMsgWnd* pWnd = new CMsgWnd();
		pWnd->SkinType = skinType;
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetMsg(text1,text2);
		return pWnd->ShowModal();
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

public:
	CDuiString SkinType;
	CButtonUI *pButton_ok;
	CButtonUI *pButton_cancel;
	CButtonUI *pButton_record;
};

