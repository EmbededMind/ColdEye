#pragma once


#include "UIlib.h"

using namespace DuiLib;

class CKeyBoardUI :public CButtonUI
{
DECLARE_DUICONTROL(CKeyBoardUI)
public:
	CKeyBoardUI();
	~CKeyBoardUI();
	void DoEvent(TEventUI &event);
};

