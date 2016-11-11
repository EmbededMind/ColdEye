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
	switch (event.Type){
	case UIEVENT_KEYDOWN: {

		}
		break;
	default:
		CCameraSwitchUI::DoEvent(event);
		break;

	}
}
