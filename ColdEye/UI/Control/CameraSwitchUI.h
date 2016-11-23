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
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetOnImage(LPCTSTR pstrValue);
	void SetOffImage(LPCTSTR pstrValue);
	void SetSwitch(bool value);
	bool GetSwitch();
	void BackPreviousItem(CTabLayoutUI*);
protected:
	bool Switch;
	CDuiString m_OnImage;
	CDuiString m_OffImage;
};

