#include "stdafx.h"
#include "CameraSwitchUI.h"
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
		switch (event.wParam)
		{
		case VK_UP:
			{
				CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
				CVerticalLayoutUI *pTopLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(0); //最上面布局
				static_cast<CCameraSwitchUI*> (pTopLayout->GetItemAt(2))->SetFocus();
			}
			break;
			
		case VK_DOWN:
			{
				CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
				CVerticalLayoutUI *pBottomLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //最底下布局
				static_cast<CCameraSwitchUI*> (pBottomLayout->GetItemAt(4))->SetFocus();
			}
			break;

		case VK_LEFT:
			if (Switch == true)
			{
				Switch = false;
				Invalidate();
			}
			break;

		case VK_RIGHT:
			if (!Switch)
			{
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
	RECT rcPos;
	CDuiString dest;
	rcPos = { 665,16,745,77};
	if (Switch == true)
	{
		dest.Format(_T("file='Button\\开关1.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}
	else
	{
		dest.Format(_T("file='Button\\开关2.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}
}