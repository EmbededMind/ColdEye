#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CMyEditUI : public CButtonUI
{
DECLARE_DUICONTROL(CMyEditUI)
public:
	CMyEditUI();
	~CMyEditUI();
	void DoEvent(TEventUI &event);
	void BackToPreviousItem(CTabLayoutUI*);
	
public:
	bool isEditing;
};

