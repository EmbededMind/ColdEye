#include "stdafx.h"

#include "Control\PopupMenuUI.h"

IMPLEMENT_DUICONTROL(CPopupMenuUI)

CPopupMenuUI::CPopupMenuUI()
{
	mHintNumber = _T("0");
}



CPopupMenuUI::~CPopupMenuUI()
{}


void CPopupMenuUI::DoEvent(TEventUI& event)
{
	int iInx;

	CContainerUI* pFocused;

	if (event.Type == UIEVENT_KEYDOWN) {
		switch (event.wParam)
		{
			case VK_SPACE:
				break;
            //---------------------------------------
			case VK_UP:
				if (mPrevItem) {
					mPrevItem->SetFocus();
				}
				break;
			//--------------------------------------
			case VK_DOWN:
				if (mNextItem) {
					mNextItem->SetFocus();
				}
				break;
			//--------------------------------------
			case VK_LEFT:
				break;
			//--------------------------------------
			case VK_RIGHT:
				iInx = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_secondmenu")))->GetCurSel();
				pFocused = (CContainerUI*)static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layour_secondmenu")))->GetItemAt(iInx);

				if (pFocused->GetCount() > 0) {
					static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SetVisible(true);
					pFocused->GetItemAt(0)->SetFocus();
					SetMenuBkColor(0xFFE6EF, 0xFFFFFFFF);
					SetBkColor(0xFF477688F);
					SetTextColor(0xFFFFFFFF);
				}
				break;
		}

		if (event.Type == UIEVENT_SETFOCUS) {
			SetBkColor(0xFF4198FE);
			static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_secondmenu")))->SelectItem(StrToInt(GetUserData()));
		}
		else if (event.Type == UIEVENT_KILLFOCUS) {
			SetBkColor(0xFFFFFFFF);
		}

		CButtonUI::DoEvent(event);
	}
}

void CPopupMenuUI::SetItemRelation(CPopupMenuUI * pPrevMenu, CPopupMenuUI * pNextMenu)
{
	mPrevItem = pPrevMenu;
	mNextItem = pNextMenu;
}

void CPopupMenuUI::SetMenuBkColor(DWORD menuColor, DWORD itemColor)
{
	static_cast<CVerticalLayoutUI*> (m_pManager->FindControl(_T("menuLayout")))->SetBkColor(menuColor);
	static_cast<CPopupMenuUI*> (m_pManager->FindControl(_T("alarmVideo")))->SetBkColor(menuColor);
	static_cast<CPopupMenuUI*> (m_pManager->FindControl(_T("setting")))->SetBkColor(menuColor);
	static_cast<CPopupMenuUI*> (m_pManager->FindControl(_T("autowatch")))->SetBkColor(menuColor);
	static_cast<CPopupMenuUI*> (m_pManager->FindControl(_T("videoget")))->SetBkColor(menuColor);
	static_cast<CPopupMenuUI*> (m_pManager->FindControl(_T("homewatch")))->SetBkColor(menuColor);

	CTabLayoutUI* pTab;
	CVerticalLayoutUI* pLayout;

	pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_secondmenu")));
	pTab->SetBkColor(itemColor);
	pLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());

	for (int i = 0; i < pLayout->GetCount(); i+=2) {
		pLayout->GetItemAt(i)->SetBkColor(itemColor);
	}

	m_pManager->GetFocus()->SetBkColor(0xFF4198FE);
}

void CPopupMenuUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);

	if (mHintNumber != _T("0")) {
		RECT rcPos = {259, 10, 294, 45};
		RECT textPos = GetPos();

		CDuiString dest;

		if (mHintNumber.GetLength() == 2) {
			textPos.left += 263;
			textPos.top += 17;
			textPos.right = textPos.left + 290;
			textPos.bottom = textPos.top + 38;
		}
		else if (mHintNumber.GetLength() == 1) {
			textPos.left += 269;
			textPos.top += 14;
			textPos.right = textPos.left + 290;
			textPos.bottom = textPos.top + 38;
		}

		dest.Format(_T("file='Button\\??????.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
		CRenderEngine::DrawText(hDC, m_pManager, textPos, mHintNumber, 0xFFFFFF, 1, 0);
	}
}

void CPopupMenuUI::SetHintNum(CDuiString num)
{
	mHintNumber = num;
	this->Invalidate();
}

CDuiString& CPopupMenuUI::GetHintNum()
{
	return mHintNumber;
}

