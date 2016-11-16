#pragma once

#include "UIlib.h"
#include "File/RecordFileInfo.h"
#include "conio.h"
using namespace DuiLib;

class CMyListUI : public CListLabelElementUI
{
	DECLARE_DUICONTROL(CMyListUI)

public:
	CMyListUI();
	CMyListUI(CRecordFileInfo& pInfo);
	~CMyListUI();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void PaintStatusImage(HDC hDC);
	void DrawItemText(HDC hDC, const RECT& rcItem);
	void DoEvent(TEventUI &event);
public:
	bool mIsLocked;
	int	prereadMessageNum;
	__time64_t mBeginTime;
	__time64_t mEndTime;
	ULONGLONG mTotalSize;
};

