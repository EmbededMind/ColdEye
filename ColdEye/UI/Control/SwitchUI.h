#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CSwitchUI : public CButtonUI
{
DECLARE_DUICONTROL(CSwitchUI)
public:
	CSwitchUI();
	~CSwitchUI();
	void DoEvent(TEventUI &event);
	void PaintStatusImage(HDC hDC);
	void SetValue(bool value);
	bool GetValue();
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetOnImage(CDuiString pstrValue);
	void SetOffImage(CDuiString pstrValue);

private:
	bool mValue;
	CDuiString sOnImage;
	CDuiString sOffImage;
};
