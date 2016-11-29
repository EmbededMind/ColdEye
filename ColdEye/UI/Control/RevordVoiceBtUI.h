#pragma once


#include "UIlib.h"
using namespace DuiLib;

class CRevordVoiceBtUI 
	:public CButtonUI
{
DECLARE_DUICONTROL(CRevordVoiceBtUI)
public:
	CRevordVoiceBtUI();
	~CRevordVoiceBtUI();
	void DoEvent(TEventUI &event);
};

