#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CMyListUI : public CListLabelElementUI
{
	DECLARE_DUICONTROL(CMyListUI)

public:
	CMyListUI();
	~CMyListUI();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void PaintStatusImage(HDC hDC);
	void DrawItemText(HDC hDC, const RECT& rcItem);
public:
	bool mIsLocked;
	int	prereadMessageNum;
	__time64_t mBeginTime;
	__time64_t mEndTime;
	ULONGLONG mTotalSize;
};

