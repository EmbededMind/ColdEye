#pragma once

#include "CameraSwitchUI.h"

class CAlarmLightUI : public CCameraSwitchUI
{
DECLARE_DUICONTROL(CAlarmLightUI)
public:
	CAlarmLightUI();
	~CAlarmLightUI();
	void DoEvent(TEventUI &event);
};

