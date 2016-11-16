#include "stdafx.h"

#include "OSDPainter.h"

COSDPainter::COSDPainter()
{}


COSDPainter::~COSDPainter()
{}

void COSDPainter::SetBitmap(CBitmap * pBitmap)
{
	pBitmap->GetBitmap(&mBitmap);
	Print("%d,%d", mBitmap.bmWidth, mBitmap.bmHeight);
}

void COSDPainter::ShowBitmap(HDC hDC)
{
	CDC* pDC = CDC::FromHandle(hDC);


	if (mSrcDC) {
		if (! pDC->BitBlt(100, 100, mBitmap.bmWidth, mBitmap.bmHeight, &mSrcDC, 0, 0, SRCCOPY)) {
			Print("GG");
		}
	}
	else if (mSrcDC == 0) {
		mSrcDC.CreateCompatibleDC(pDC);

		mSrcDC.SelectObject(pBitmap);

		pDC->BitBlt(100, 100, mBitmap.bmWidth, mBitmap.bmHeight, &mSrcDC, 0, 0, SRCCOPY);
	}
}





