#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CCameraSwitchUI :public CButtonUI
{
DECLARE_DUICONTROL(CCameraSwitchUI)
public:
	CCameraSwitchUI();
	~CCameraSwitchUI();
	void DoEvent(TEventUI &event);
	void PaintStatusImage(HDC hDC);

public:
	bool Switch;
};

