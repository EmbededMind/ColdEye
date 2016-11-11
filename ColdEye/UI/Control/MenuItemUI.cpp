#include "stdafx.h"

#include "Control\MenuItemUI.h"
#include "Control\MyEditUI.h"

IMPLEMENT_DUICONTROL(CMenuItemUI)

CMenuItemUI::CMenuItemUI()
{}

CMenuItemUI::CMenuItemUI(CContainerUI* pParent, CDuiString& text)
{
	mHintNumber = _T("0");
	SetText(text);

	pParent->Add(this);
	SetAttribute(_T("style"), _T("MenuItem"));

	CControlUI * p = new CControlUI;
	pParent->Add(p);
	p->SetAttribute(_T("style"), _T("control"));
}


CMenuItemUI::CMenuItemUI(CContainerUI* pParent, CDuiString& text, int iInx)
{
	mHintNumber = _T("0");

	SetText(text);
	pParent->AddAt(this, iInx);
	SetAttribute(_T("style"), _T("MenuItem"));

	CControlUI* p = new CControlUI;
	pParent->AddAt(p, iInx+1);
	p->SetAttribute(_T("style"), _T("control"));
}

CMenuItemUI::~CMenuItemUI()
{}


void CMenuItemUI::SetItemBkColor(DWORD color1, DWORD color2)
{
	CTabLayoutUI* pTab;
	CVerticalLayoutUI* pLayout;

	pTab = static_cast<CTabLayoutUI*> (m_pManager->FindControl(_T("layout_secondmenu")));
	pTab->SetBkColor(color1);

	pLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());

	for (int i = 0; i < pLayout->GetCount(); i+=2){
		pLayout->GetItemAt(i)->SetBkColor(0xFFEFEFF4);
	}

	pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
	pTab->SetBkColor(color2);
}



void CMenuItemUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN) {
		switch (event.wParam)
		{
			case VK_UP:
				if (mPrevItem) {
					mPrevItem->SetFocus();
				}
				break;
		    //----------------------------------------------------
			case VK_DOWN:
				if (mNextItem) {
					mNextItem->SetFocus();
				}
				break;
 			//----------------------------------------------------
			case VK_LEFT:
				{
					CPopupMenuUI* pFocusedItem = NULL;
					static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SetVisible(false);
					int userData = StrToInt(GetUserData());

					if (userData < 6) {
						pFocusedItem = static_cast<CPopupMenuUI*>(m_pManager->FindControl(_T("alarmvideo")));
					}
					else if (userData > 6  &&  userData < 14) {
						pFocusedItem = static_cast<CPopupMenuUI*>(m_pManager->FindControl(_T("setting")));
					}
					else if (userData >= 14 && userData < 18) {
						pFocusedItem = static_cast<CPopupMenuUI*>(m_pManager->FindControl(_T("autowatch")));
					}
					else if (userData >= 18 && userData < 25) {
						pFocusedItem = static_cast<CPopupMenuUI*>(m_pManager->FindControl(_T("videoget")));
					}

					if (pFocusedItem) {
						pFocusedItem->SetTextColor(0xFF666666);
						m_pManager->SetFocus(pFocusedItem);
						SetItemBkColor(0xFFFFFFFF, 0xFFE6E6EF);
					}
				}
				break;
			//----------------------------------------------------------------------
			case VK_RIGHT:
				{
					int userData = StrToInt(GetUserData());
					int sel;
					switch (userData)
					{
						case 6:
							static_cast<CControlUI*>(m_pManager->FindControl(_T("shipname")))->SetFocus();
							break;
					    //---------------
						case 7:
						case 8:
						case 9:
						case 10:
						case 11:
						case 12:
							{
								CTabLayoutUI* pThirdMenu;
								CVerticalLayoutUI* pChildLayout;
								CControlUI* pControl;
								pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
								sel = pThirdMenu->GetCurSel();
								pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
								pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
								pControl = pChildLayout->GetItemAt(2);
								pControl->SetFocus();
							}
							break;
						//------------------------------------------------------------
						case 15:
							{							
								CTabLayoutUI* pThirdMenu;
								CVerticalLayoutUI* pChildLayout;
								CControlUI* pItem;
								pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
								sel = pThirdMenu->GetCurSel();

								pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
								pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
								pItem = pChildLayout->GetItemAt(2);
								pItem->SetFocus();
							}
							break;
						//-------------------------------------------
					}
					SetItemBkColor(0xFFEFEFF4, 0xFFFFFFFF);
					SetBkColor(0xFF4178B7);
					SetTextColor(0xFFFFFFFF);
				}
				break;
		}
	}

	if (event.Type == UIEVENT_SETFOCUS) {
		SetBkColor(0xFF4198FE);
		static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SelectItem(StrToInt(GetUserData()));
	}
	else {
		SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}