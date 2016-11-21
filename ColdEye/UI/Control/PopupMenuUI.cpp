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
			pFocused = (CContainerUI*)static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_secondmenu")))->GetItemAt(iInx);

			if (GetUserData() == _T("4")){
				m_pManager->FindControl(_T("button_home"))->SetFocus();
			}
			else if (pFocused->GetCount() > 0) {
					static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SetVisible(true);
					pFocused->GetItemAt(0)->SetFocus();
					SetMenuBkColor(0xFFE6E6E6, 0xFFFFFFFF);
			}
			break;
		}
	}

	if (event.Type == UIEVENT_SETFOCUS) {
		SetBkColor(0xFF4198FE);
		if (GetUserData() == _T("4")) //»Ø¼Ò¿´´¬
		{
			m_pManager->FindControl(_T("home"))->SetVisible(true);
		}
		else
		{
			if (m_pManager->FindControl(_T("home"))->IsVisible())
			{
				m_pManager->FindControl(_T("home"))->SetVisible(false);
			}
			static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_secondmenu")))->SelectItem(StrToInt(GetUserData()));
		}
	}
	else if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}

void CPopupMenuUI::SetItemRelation(CPopupMenuUI * pPrevMenu, CPopupMenuUI * pNextMenu)
{
	mPrevItem = pPrevMenu;
	mNextItem = pNextMenu;
}

void CPopupMenuUI::SetMenuBkColor(DWORD menuColor, DWORD itemColor)
{
	CContainerUI *layout;
	layout = (CContainerUI*)GetParent();
	layout->SetBkColor(menuColor);
	for (int i = 0; i < layout->GetCount(); i+=2)
		layout->GetItemAt(i)->SetBkColor(menuColor);

	SetBkColor(0xFF47688F);
	SetTextColor(0xFFFFFFFF);

	CControlUI *pItem;
	pItem =m_pManager->GetFocus();
	layout = (CContainerUI*)pItem->GetParent();
	layout->SetBkColor(itemColor);
	for (int i = 0; i < layout->GetCount(); i += 2) {
		if(layout->GetItemAt(i)!= pItem)
			layout->GetItemAt(i)->SetBkColor(itemColor);
	}
	
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

void CPopupMenuUI::SetPrevItem(CControlUI * PrevItem)
{
	mPrevItem = PrevItem;
}

void CPopupMenuUI::SetNextItem(CControlUI * NextItem)
{
	mNextItem = NextItem;
}

CControlUI * CPopupMenuUI::GetPrevItem()
{
	return mPrevItem;
}

CControlUI * CPopupMenuUI::GetNextItem()
{
	return mNextItem;
}

CDuiString& CPopupMenuUI::GetHintNum()
{
	return mHintNumber;
}

