#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CRecordvoiceUI : 
	public CLabelUI
{
DECLARE_DUICONTROL(CRecordvoiceUI)
public:
	CRecordvoiceUI();
	~CRecordvoiceUI();
	virtual void PaintStatusImage(HDC hDC);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetRecordindImage(LPCTSTR pstrValue);

public:
	bool Blink;
	UINT8 RecordTime;
private:
	CDuiString m_recordingImage;
};

