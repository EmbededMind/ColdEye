#pragma once

#include "UIlib.h"
#include "Control\PopupMenuUI.h"
#include "File\RecordFileInfo.h"
#include <list>

using namespace DuiLib;

class CMenuItemUI :
	public CPopupMenuUI
{
	DECLARE_DUICONTROL(CMenuItemUI)

public:
	CMenuItemUI();
	CMenuItemUI(CContainerUI* pParent, CString& text);
	CMenuItemUI(CContainerUI* pParent, CString& text, CDuiString userdata,int iInx);
	~CMenuItemUI();

	void SetItemBkColor(CControlUI* pfocusItem,DWORD Color1, DWORD Color2);

	virtual void DoEvent(TEventUI& event);
	virtual LPCTSTR GetClass();

	list<CRecordFileInfo*> FindRecordFile(UINT8*);
	void HardDriverStatus(list<CRecordFileInfo*> , UINT8);
	//void SetItemRelation(CMenuItemUI* pPrevItem, CMenuItemUI* pNextItem);
	
//private:
//	CMenuItemUI* mPrevItem;
//	CMenuItemUI* mNextItem;
};