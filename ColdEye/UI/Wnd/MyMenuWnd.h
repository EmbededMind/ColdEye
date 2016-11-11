#pragma once

#include "UIlib.h"

using namespace DuiLib;

class CMyMenuWnd :
	public WindowImplBase
{
public :
	CMyMenuWnd();
	~CMyMenuWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();


	CVerticalLayoutUI* menuLayout;
	CTabLayoutUI*  pTab[2];
	

	void InitWindow();
	void OnFinalMessage(HWND hWnd);
	void Notify(TNotifyUI& msg);
	void OnLClick(CControlUI* pControl);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void AddCamear();


};