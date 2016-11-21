#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CSwitchRecordListUI
	:public CListLabelElementUI
{
public:
	CSwitchRecordListUI();
	CSwitchRecordListUI(CTime ,CDuiString);
	~CSwitchRecordListUI();
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	virtual void DrawItemText(HDC hDC, const RECT& rcItem);
	virtual void DrawItemBk(HDC hDC, const RECT& rcItem);
	void SetBkColor(DWORD);
private:
	DWORD m_itemBk;
	DWORD m_selectedItemBk;
	CTime m_time;
	CDuiString m_recordType;
};