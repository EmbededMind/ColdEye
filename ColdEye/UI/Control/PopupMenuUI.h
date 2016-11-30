#pragma once

#include "UIlib.h"

using namespace DuiLib;

class CPopupMenuUI :
	public CButtonUI
{
	DECLARE_DUICONTROL(CPopupUMenuUI)
public:
	CPopupMenuUI();
	~CPopupMenuUI();

	CPopupMenuUI* mLastFocusedMenu;

	virtual void DoEvent(TEventUI& event);
	virtual void PaintStatusImage(HDC hDC);

	void SetMenuBkColor(DWORD menuColor, DWORD itemColor);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetMark(LPCTSTR pstrValue);
	void SetHintNum(int num);
	int GetHintNum();

protected:
	int isMark;
	int mHintNumber;
	int ButtonId;
};
