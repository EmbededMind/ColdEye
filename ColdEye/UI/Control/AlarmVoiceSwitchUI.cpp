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
	case UIEVENT_KEYDOWN:{
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
			CVerticalLayoutUI *pChildLayout = (CVerticalLayoutUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());
			pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(1);
			switch (event.wParam){
			case VK_DOWN:
				if (Switch){
					pChildLayout->GetItemAt(2)->SetFocus();
				}
				break;
			case VK_LEFT:
				if (Switch){
					Switch = false;
					pChildLayout->SetVisible(false);
				}
				break;

			case VK_RIGHT:
				if (!Switch){
					Switch = true;
					pChildLayout->SetVisible(true);
				}
				break;

			case VK_BACK:
				m_pManager->FindControl(_T("alarm_voice"))->SetFocus();
				break;
			}
		}
		break;

	default:
		CCameraSwitchUI::DoEvent(event);
		break;
	}
}
