#pragma once

#include "UIlib.h"
using namespace DuiLib;

class CMyLabelUI :public CButtonUI
{
DECLARE_DUICONTROL(CMyLabelUI)
public:
	CMyLabelUI();
	~CMyLabelUI();
	void PaintText(HDC hDC);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetStateEnableText(LPCTSTR pstrText);
	void SetStateDisableText(LPCTSTR pstrText);
	void SetStateTextColor(DWORD Color);
	void SetStateTextFocusColor(DWORD Color);
	void PaintStatusImage(HDC hDC);
	void PaintBkColor(HDC hDC);
	void SetFocusBkColor(DWORD Color);
	void SetSelBkImage(LPCTSTR pstrValue);
	void SetFocusedSelBkImage(LPCTSTR pstrValue);
	void DoEvent(TEventUI &event);
	void StatusUpdate();
public:
	CDuiString m_stateEnableText;
	CDuiString m_stateDisableText;
	DWORD m_stateTextColor;
	DWORD m_stateTextFocusColor;
	DWORD m_focusBkColor;
	bool Value;
	CDuiString m_selbkimage;
	CDuiString m_focusedselbkimage;
};

