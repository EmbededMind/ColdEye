#include "stdafx.h"
#include "CameraSwitchUI.h"
#include "Wnd\MsgWnd.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CCameraSwitchUI)
CCameraSwitchUI::CCameraSwitchUI()
{
	Switch = false;
}


CCameraSwitchUI::~CCameraSwitchUI()
{
}


void CCameraSwitchUI::DoEvent(TEventUI &event)
{ 
	if (event.Type == UIEVENT_KEYDOWN)
	{
		CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
		switch (event.wParam)
		{
		case VK_UP:{
				CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());
				CVerticalLayoutUI *pTopLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(0); //最上面布局
				pTopLayout->GetItemAt(2)->SetFocus();
			}
			break;
			
		case VK_DOWN:{
				CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());
				CVerticalLayoutUI *pBottomLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //最底下布局
				pBottomLayout->GetItemAt(4)->SetFocus();
			}
			break;

		case VK_LEFT:
			if (Switch == true){
				//if(MSGID_OK == CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_camera_switch.xml"), NULL, NULL))
				SendMessage(m_pManager->GetPaintWindow(), USER_MSG_MESSAGE_BOX, CLOSE_CAMERA, 0);
				//m_pManager->SendNotify(this, USER_MSG_MESSAGEBOX);
				Switch = false;
				Invalidate();
			}
			break;

		case VK_RIGHT:
			if (!Switch){
				Switch = true;
				Invalidate();
			}
			break;

		case VK_BACK:
			BackPreviousItem(pTabLayout);
			break;
		}
	}

	if (event.Type == UIEVENT_BUTTONDOWN)
	{
		if (!Switch)
			Switch = true;
		else
			Switch = false;
		Invalidate();
	}

	if (event.Type == UIEVENT_SETFOCUS)
	{
		SetBkColor(0xFF4198FE);

	}
	else if (event.Type == UIEVENT_KILLFOCUS)
	{
		SetBkColor(0xFFF3F3F3);
	}
	CButtonUI::DoEvent(event);
}

void CCameraSwitchUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);
	if (Switch == true){
		DrawImage(hDC,m_OnImage);
	}
	else{
		DrawImage(hDC, m_OffImage);
	}
}

void CCameraSwitchUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("onimage")) == 0) 
		SetOnImage(pstrValue);
	else if (_tcsicmp(pstrName, _T("offimage")) == 0) 
		SetOffImage(pstrValue);
	CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CCameraSwitchUI::SetOnImage(LPCTSTR pstrValue)
{
	m_OnImage = pstrValue;
}

void CCameraSwitchUI::SetOffImage(LPCTSTR pstrValue)
{
	m_OffImage = pstrValue;
}

void CCameraSwitchUI::SetSwitch(bool value)
{
	Switch = value;
}

bool CCameraSwitchUI::GetSwitch()
{
	return Switch;
}

void CCameraSwitchUI::BackPreviousItem(CTabLayoutUI* pTabLayout)
{
	int sel,count,focus_userdata;
	sel = pTabLayout->GetCurSel();
	CControlUI *pItem;
	if (sel > 5 && sel <= 13) { //主机设置菜单项
		CTabLayoutUI *pPrevLayout=static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_submenu_setting")));
		count = pPrevLayout->GetCount();
		for (int i=0; i < count; i += 2) {
			pItem = pPrevLayout->GetItemAt(i);
			focus_userdata = StrToInt(pItem->GetUserData());
			if (sel == focus_userdata) {
				pItem->SetFocus();
				break;
			}
		}
	}

}
