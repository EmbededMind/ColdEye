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
public:
	CSliderUI *pSlider;
	CTabLayoutUI *pTabLayout;
	CPaintManagerUI *m_paintManger;
	CRect m_rc;
};

