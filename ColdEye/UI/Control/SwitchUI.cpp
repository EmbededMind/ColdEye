#include "stdafx.h"
#include "SwitchUI.h"

IMPLEMENT_DUICONTROL(CSwitchUI)
CSwitchUI::CSwitchUI()
	:mValue(false)
{
}


CSwitchUI::~CSwitchUI()
{
}

void CSwitchUI::DoEvent(TEventUI & event)
{
	switch (event.Type)
	{
	case UIEVENT_KEYDOWN:
		m_pManager->SendNotify(this,DUI_MSGTYPE_SWITCH,event.wParam,event.lParam);
		break;

	case UIEVENT_SETFOCUS:
		SetBkColor(0xFF4198FE);
		break;

	case UIEVENT_KILLFOCUS:
		SetBkColor(0xFFF3F3F3);
		break;
	}
	CButtonUI::DoEvent(event);
}

void CSwitchUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);
	if (mValue == true) {
		DrawImage(hDC, sOnImage);
	}
	else {
		DrawImage(hDC, sOffImage);
	}
}

void CSwitchUI::SetValue(bool value)
{
	mValue = value;
}

bool CSwitchUI::GetValue()
{
	return mValue;
}

void CSwitchUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("onimage")) == 0)
		SetOnImage(pstrValue);
	else if (_tcsicmp(pstrName, _T("offimage")) == 0)
		SetOffImage(pstrValue);
	CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CSwitchUI::SetOnImage(CDuiString pstrValue)
{
	sOnImage = pstrValue;
}

void CSwitchUI::SetOffImage(CDuiString pstrValue)
{
	sOffImage = pstrValue;
}

