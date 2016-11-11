#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CAlarmVoiceListUI : public CButtonUI
{
DECLARE_DUICONTROL(CAlarmVoiceListUI)
public:
	CAlarmVoiceListUI();
	~CAlarmVoiceListUI();
	void PaintBkColor(HDC hDC);
	void PaintStatusImage(HDC hDC);
	void DoEvent(TEventUI &event);
public:
	bool state;
	CRect m_rc;
};

