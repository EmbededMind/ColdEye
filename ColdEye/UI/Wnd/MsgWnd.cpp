#include "stdafx.h"
#include "MsgWnd.h"


DUI_BEGIN_MESSAGE_MAP(CMsgWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CMsgWnd::CMsgWnd()
{
}


CMsgWnd::~CMsgWnd()
{
}


void CMsgWnd::SetTitle(LPCTSTR lpstrTitle)
{
	if (lstrlen(lpstrTitle) <= 0) return;
/*
	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	if (pControl) pControl->SetText(lpstrTitle);*/
}

void CMsgWnd::SetMsg(LPCTSTR lpstrMsg)
{
	if (lstrlen(lpstrMsg) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageText")));
	if (pControl) pControl->SetText(lpstrMsg);
}

void CMsgWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CMsgWnd::GetSkinFile()
{
	return _T("msg.xml");
}

LPCTSTR CMsgWnd::GetWindowClassName(void) const
{
	return _T("MsgWnd");
}

void CMsgWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
}

LRESULT CMsgWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CMsgWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CMsgWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CMsgWnd::InitWindow()
{
	/*m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));*/
}
