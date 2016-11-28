#include "stdafx.h"
#include "AlphaMarkWnd.h"

CAlphaMarkWnd::CAlphaMarkWnd()
{
}

CAlphaMarkWnd::CAlphaMarkWnd(CDuiString skinName)
{
	if (skinName)
		mSkinFileName = skinName;
}

CAlphaMarkWnd::~CAlphaMarkWnd()
{
}

LPCTSTR CAlphaMarkWnd::GetWindowClassName() const
{
	return _T("AlphaMarkWnd");
}

CDuiString CAlphaMarkWnd::GetSkinFile()
{
	if (mSkinFileName)
		return mSkinFileName;
}


LRESULT CAlphaMarkWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	CDuiString PlayText;
	switch (uMsg) {
	case USER_MSG_PLAY_PLAY:
		ShowWindow(false);
		break;
		
	case USER_MSG_PLAY_SLOW:
		PlayText.Format(_T("X%d"), wParam);
		pBt_fast->SetVisible(false);
		pBt_slow->SetVisible(true);
		pBt_slow->SetText(PlayText);
		break;

	case USER_MSG_PLAY_FAST:
		PlayText.Format(_T("X%d"), wParam);
		pBt_slow->SetVisible(false);
		pBt_fast->SetVisible(true);
		pBt_fast->SetText(PlayText);
		break;
	}
	return LRESULT();
}

void CAlphaMarkWnd::Notify(TNotifyUI & msg)
{
}

void CAlphaMarkWnd::InitWindow()
{
	pBt_fast = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_fast")));
	pBt_slow = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_slow")));
}
