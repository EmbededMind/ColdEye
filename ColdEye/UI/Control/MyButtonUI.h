#pragma once


#include "UIlib.h"
using namespace DuiLib;

class CMyButtonUI 
	:public CButtonUI
{
DECLARE_DUICONTROL(CMyButtonUI)
public:
	CMyButtonUI();
	~CMyButtonUI();
	void DoEvent(TEventUI &event);
};

