#pragma once

#include "UILib.h"

#include "Wnd\WallWnd.h"
#include "Wnd\MyMenuWnd.h"

using namespace DuiLib;

class CMainWnd : public CWindowWnd, public INotifyUI
{
public:
	CMainWnd();
	~CMainWnd();

	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void    Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	//virtual CDuiString GetSkinFile() { return _T("main.xml"); }
	//virtual CDuiString GetSkinFolder() { return _T(""); }


protected:
	CPaintManagerUI m_PaintManager;

private:
	CWallWnd * m_pWall;
	CMyMenuWnd* m_pMenu;
};

