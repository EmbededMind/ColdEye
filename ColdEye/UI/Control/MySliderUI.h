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
	virtual void SetAttribute(LPCTSTR strName, LPCTSTR strValue);
	//void PaintBkColor(HDC hDC);
	//void PaintBkImage(HDC hDC);

public:
	CLabelUI* bindControl;
	CDuiString m_focusedbkimage;
	CDuiString m_bkimage;
};

