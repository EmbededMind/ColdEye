#include "stdafx.h"
#include "CameraSwitchUI.h"
#include "Wnd\MsgWnd.h"

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
