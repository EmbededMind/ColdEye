#pragma once

#include "UIlib.h"
using namespace DuiLib;


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

