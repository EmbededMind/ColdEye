#pragma once

#include "CameraSwitchUI.h"

class CAlarmVoiceSwitchUI : public CCameraSwitchUI
{
DECLARE_DUICONTROL(CAlarmVoiceSwitchUI)
public:
	CAlarmVoiceSwitchUI();
	~CAlarmVoiceSwitchUI();
	void DoEvent(TEventUI &event);
};
