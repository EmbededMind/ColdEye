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

	
	void SetHintNum(int num);
	int GetHintNum();

protected:
	int mHintNumber;
	int ButtonId;
};
