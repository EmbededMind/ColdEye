#pragma once
#include "stdafx.h"


class COSDPainter
{
public:
	COSDPainter();
	~COSDPainter();

	CDC       mSrcDC;
	HDC       hDstDC;
	CBitmap*  pBitmap;
	BITMAP    mBitmap;
	

public:
	void SetBitmap(CBitmap* pBitmap);
	void ShowBitmap(HDC hDC);
};