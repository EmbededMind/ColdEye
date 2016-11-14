#pragma once

#include "UIlib.h"
#include "Control\PopupMenuUI.h"

using namespace DuiLib;

class CMenuItemUI :
	public CPopupMenuUI
{
	DECLARE_DUICONTROL(CMenuItemUI)

public:
	CMenuItemUI();
	CMenuItemUI(CContainerUI* pParent, CDuiString& text);
	CMenuItemUI(CContainerUI* pParent, CDuiString& text, int iInx);	
	~CMenuItemUI();

	void SetItemBkColor(DWORD color_1, DWORD color_2);

	virtual void DoEvent(TEventUI& event);

	//void SetItemRelation(CMenuItemUI* pPrevItem, CMenuItemUI* pNextItem);
	
//private:
//	CMenuItemUI* mPrevItem;
//	CMenuItemUI* mNextItem;
};