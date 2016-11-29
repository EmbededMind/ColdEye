#pragma once

#include "UIlib.h"
using namespace DuiLib;


class CShipNameItemUI : public CButtonUI
{
DECLARE_DUICONTROL(CShipNameItemUI)
public:
	CShipNameItemUI();
	~CShipNameItemUI();
	void DoEvent(TEventUI &event);
};

