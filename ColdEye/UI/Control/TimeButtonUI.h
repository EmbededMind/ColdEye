#pragma once


#include "UIlib.h"
using namespace DuiLib;

class CTimeButtonUI :
	public CButtonUI
{
	DECLARE_DUICONTROL(CTimeButtonUI)

public:
	CTimeButtonUI();
	~CTimeButtonUI();

	CControlUI* mPrevItem;
	CControlUI* mNextItem;

	virtual LPCTSTR GetClass() const;

	void SetItemRelation(CTimeButtonUI* pPrevItem, CTimeButtonUI* pNextItem);
	void DecreaseHour();
	void DecreaseMinute();
	void IncreaseHour();
	void IncreaseMinute();
	bool isMorrow();
	void SetTime();

	void DoEvent(TEventUI& event);


private:
	DWORD  tMinute_1;
	DWORD  tMinute_2;
};