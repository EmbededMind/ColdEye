#include "stdafx.h"
#include "AWOnOffListLabelUI.h"

CAWOnOffListLabelUI::CAWOnOffListLabelUI()
{
}

CAWOnOffListLabelUI::CAWOnOffListLabelUI(CTime time, CDuiString type)
	:m_selectedItemBk(0xFF4198FE)
{
	SetText(time.Format("%Y-%m-%d %H:%M"));
	m_recordType = type;
}

CAWOnOffListLabelUI::~CAWOnOffListLabelUI()
{
}

void CAWOnOffListLabelUI::DoPaint(HDC hDC, const RECT & rcPaint)
{
	DrawItemBk(hDC, rcPaint);
	DrawItemText(hDC, m_rcItem);
}

void CAWOnOffListLabelUI::DrawItemText(HDC hDC, const RECT & rcItem)
{	
	CListLabelElementUI::DrawItemText(hDC, rcItem);
	CDuiString sText = GetText();
	UINT TextStyle;
	if (sText.IsEmpty()) return;

	if (m_pOwner == NULL) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	DWORD iTextColor = pInfo->dwTextColor;
	if ((m_uButtonState & UISTATE_HOT) != 0) {
		iTextColor = pInfo->dwHotTextColor;
	}
	if (IsSelected()) {
		iTextColor = pInfo->dwSelectedTextColor;
	}
	if (!IsEnabled()) {
		iTextColor = pInfo->dwDisabledTextColor;
	}
	int nLinks = 0;
	RECT rcText = rcItem;
	rcText.right = rcText.right - 45;

	TextStyle = pInfo->uTextStyle;
	TextStyle &= ~(DT_LEFT | DT_CENTER);
	TextStyle |= DT_RIGHT;

	CRenderEngine::DrawText(hDC, m_pManager, rcText, m_recordType, iTextColor, \
			pInfo->nFont, TextStyle);
}

void CAWOnOffListLabelUI::DrawItemBk(HDC hDC, const RECT & rcItem)
{
	ASSERT(m_pOwner);
	if (m_pOwner == NULL) return;
	DWORD iBackColor = 0;
	if (IsSelected()) {
		iBackColor = m_selectedItemBk;
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(iBackColor));
	}
	else {
		iBackColor = m_itemBk;
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(iBackColor));
	}
}

void CAWOnOffListLabelUI::SetBkColor(DWORD color)
{
	m_itemBk = color;
}

void CAWOnOffListLabelUI::DoEvent(TEventUI & event)
{
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		m_pManager->SendNotify(this, DUI_MSGTYPE_LISTLABEL,event.wParam,event.lParam);
		break;
	default:
		CListLabelElementUI::DoEvent(event);
		break;
	}
}

