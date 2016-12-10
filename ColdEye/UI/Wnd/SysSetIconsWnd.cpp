#include "stdafx.h"
#include "SysSetIconsWnd.h"

CSysSetIconsWnd::CSysSetIconsWnd()
{
}

CSysSetIconsWnd::CSysSetIconsWnd(CDuiString skinName)
{
	if (skinName)
		mSkinFileName = skinName;
}

CSysSetIconsWnd::~CSysSetIconsWnd()
{
}

LPCTSTR CSysSetIconsWnd::GetWindowClassName() const
{
	return _T("SysSetWnd");
}

CDuiString CSysSetIconsWnd::GetSkinFile()
{
	if (mSkinFileName)
		return mSkinFileName;
}


LRESULT CSysSetIconsWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	Print("uMsg:%x", uMsg);
	switch(uMsg){
	case WM_ACTIVATEAPP:
		if (IsWindowVisible(m_hWnd)) {
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		break;

	case WM_SHOWWINDOW:
		mshowTime = 3;
		SetTimer(m_hWnd, 1, 1000, NULL);
		break;

	case SWP_HIDEWINDOW:
		mshowTime = 3;
		KillTimer(m_hWnd, 1);
		break;

	case USER_MSG_SYS_VOLUM:
		if (!IsWindowVisible(m_hWnd)) {
			ShowWindow(true);
		}
		pVolume->SetVisible(true);
		pLight->SetVisible(false);
		mshowTime = 3;
		break;
		
	case USER_MSG_SYS_LIGHT:
		pVolume->SetVisible(false);
		pLight->SetVisible(true);
		mshowTime = 3;
		break;

	case WM_TIMER:
		if (!mshowTime) {
			ShowWindow(false);
		}
		else mshowTime--;
		break;
	}
	bHandled = FALSE;
	return 0;
}

void CSysSetIconsWnd::Notify(TNotifyUI & msg)
{
}

void CSysSetIconsWnd::InitWindow()
{
	m_pm.SetDPI(mDpi);
	pVolume = static_cast<CButtonUI*>(m_pm.FindControl(_T("volume")));
	pLight = static_cast<CButtonUI*>(m_pm.FindControl(_T("light")));
}



void CSysSetIconsWnd::SetDpi(int dpi)
{
	mDpi = dpi;
}
