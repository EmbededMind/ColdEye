#pragma once

#include "UIlib.h"
using namespace DuiLib;

#define DUI_USER_MSGTYPE_BACK      (_T("back"))

class CMySliderUI : public CSliderUI
{
DECLARE_DUICONTROL(CMySliderUI)

public:
	CMySliderUI();
	~CMySliderUI();
	void DoEvent(TEventUI &event);
	void PaintBkColor(HDC hDC);
	void PaintBkImage(HDC hDC);
	void BackPreviousItem(CTabLayoutUI*);
};

