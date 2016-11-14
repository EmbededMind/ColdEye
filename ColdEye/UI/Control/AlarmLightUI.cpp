#include "stdafx.h"
#include "AlarmLightUI.h"

IMPLEMENT_DUICONTROL(CAlarmLightUI)

CAlarmLightUI::CAlarmLightUI()
{
}


CAlarmLightUI::~CAlarmLightUI()
{
}

void CAlarmLightUI::DoEvent(TEventUI & event)
{
	switch (event.Type) {
		case UIEVENT_KEYDOWN: {
			if (event.wParam == VK_BACK) {
				m_pManager->FindControl(_T("alarm_light"))->SetFocus();
			}
			else if(event.wParam == VK_LEFT || event.wParam == VK_RIGHT){
				CCameraSwitchUI::DoEvent(event);
			}
			break;
		default:
			CCameraSwitchUI::DoEvent(event);
			break;
		}
	}
}
