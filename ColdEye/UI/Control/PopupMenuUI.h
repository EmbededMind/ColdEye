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

	void SetItemRelation(CPopupMenuUI* pPrevMenu, CPopupMenuUI* pNextMenu);
	void SetMenuBkColor(DWORD menuColor, DWORD itemColor);

	
	void SetHintNum(int num);
	int GetHintNum();
	void SetPrevItem(CControlUI *PrevItem);
	void SetNextItem(CControlUI *NextItem);
	CControlUI* GetPrevItem();
	CControlUI* GetNextItem();

protected:
	int mHintNumber;
	int ButtonId;
	CControlUI* mPrevItem;
	CControlUI* mNextItem;
};
