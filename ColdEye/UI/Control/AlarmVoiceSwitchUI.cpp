#include "stdafx.h"
#include "AlarmVoiceSwitchUI.h"

IMPLEMENT_DUICONTROL(CAlarmVoiceSwitchUI)
CAlarmVoiceSwitchUI::CAlarmVoiceSwitchUI()
{
}


CAlarmVoiceSwitchUI::~CAlarmVoiceSwitchUI()
{
}

void CAlarmVoiceSwitchUI::DoEvent(TEventUI & event)
{
	switch (event.Type)
	{
	case UIEVENT_KEYDOWN:
		switch (event.wParam)
		{
		case VK_LEFT:
			if (Switch)
			{
				Switch = false;
				CTabLayoutUI *pThirdMenu;
				CVerticalLayoutUI *pChildLayout;
				pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(pThirdMenu->GetCurSel());
				pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(1);
				pChildLayout->SetVisible(false);
			}
			break;

		case VK_RIGHT:
			if (!Switch)
			{
				Switch = true;
				CTabLayoutUI *pThirdMenu;
				CVerticalLayoutUI *pChildLayout;
				pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(pThirdMenu->GetCurSel());
				pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(1);
				pChildLayout->SetVisible(true);
			}
			break;
		}
		break;

	default:
		CCameraSwitchUI::DoEvent(event);
		break;
	}
}
