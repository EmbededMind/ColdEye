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
					else if (userData >= 6  &&  userData < 14) {
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
				int userdata = StrToInt(GetUserData());
				int sel;
				CTabLayoutUI *pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				CVerticalLayoutUI *pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(pThirdMenu->GetCurSel());

				if (userdata >= 0 && userdata < 6)//报警视频列表
				{
					CDuiString name;
					CListUI *pList;
					name.Format(_T("list%d"), userdata + 1);
					pList = static_cast<CListUI*>(m_pManager->FindControl(name));
					int Count = pList->GetCount();
					if (Count > 0)
					{
						pList->GetItemAt(0)->SetFocus();
						pList->SelectItem(0);
					}
				}
				else if (userdata == 6)//主机名称
				{
					m_pManager->FindControl(_T("edit_shipname"))->SetFocus();
				}
				else if (userdata >= 7 && userdata < 13)//摄像机设置
				{
					CButtonUI *pEdit;
					sel = pThirdMenu->GetCurSel();
					pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
					pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
					pEdit = (CButtonUI*)pChildLayout->GetItemAt(2); //船名编辑框
					pEdit->SetFocus();
				}
				else if (userdata == 13) //系统设置
				{
					pChildLayout->GetItemAt(2)->SetFocus();
				}
				else if (userdata == 14)//看船时间
				{
					static_cast<CButtonUI*>(m_pManager->FindControl(_T("time1_hour")))->SetFocus();
				}
				else if (userdata == 15)//报警音
				{
					CButtonUI *pAlarmVoiceSwitch;
					sel = pThirdMenu->GetCurSel();
					pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
					pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
					pAlarmVoiceSwitch = (CButtonUI*)pChildLayout->GetItemAt(2); //报警音开关
					pAlarmVoiceSwitch->SetFocus();
				}
				else if (userdata == 16)//警告灯光
				{
					m_pManager->FindControl(_T("alarmlight_switch"))->SetFocus();
				}
				else if (userdata == 17) //看船开关记录
				{

				}
				SetItemBkColor(0xFFEFEFF4, 0xFFFFFFFF);
				SetBkColor(0xFF4178B7);
				SetTextColor(0xFFFFFFFF);
				break;
		}
	}

	if (event.Type == UIEVENT_SETFOCUS) {
		SetBkColor(0xFF4198FE);
		static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SelectItem(StrToInt(GetUserData()));
	}
	else if(event.Type == UIEVENT_KILLFOCUS){
		SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}