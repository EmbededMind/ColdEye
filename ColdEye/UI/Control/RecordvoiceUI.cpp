#include "stdafx.h"
#include "RecordvoiceUI.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CRecordvoiceUI)
CRecordvoiceUI::CRecordvoiceUI()
	:Blink(true),
	RecordTime(60)
{
}


CRecordvoiceUI::~CRecordvoiceUI()
{
}

void CRecordvoiceUI::PaintStatusImage(HDC hDC)
{
	CLabelUI::PaintStatusImage(hDC);
	if (Blink){
		DrawImage(hDC, m_recordingImage);
	}
}

void CRecordvoiceUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("recordingimage")) == 0)
		SetRecordindImage(pstrValue);
	CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CRecordvoiceUI::SetRecordindImage(LPCTSTR pstrValue)
{
	m_recordingImage = pstrValue;
}
