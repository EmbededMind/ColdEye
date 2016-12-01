#include "stdafx.h"

#include "Control\PopupMenuUI.h"
#include "Device\Port.h"

IMPLEMENT_DUICONTROL(CPopupMenuUI)

CPopupMenuUI::CPopupMenuUI()
	:mHintNumber(0),
	isMark(false)
{
}



CPopupMenuUI::~CPopupMenuUI()
{}


void CPopupMenuUI::DoEvent(TEventUI& event)
{
	int iInx;

	CContainerUI* pFocused;

	if (event.Type == UIEVENT_KEYDOWN) {
			m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
	}

	if (event.Type == UIEVENT_SETFOCUS) {
		SetBkColor(0xFF4198FE);
		if (GetUserData() == _T("4")) //回家看船
		{
			m_pManager->FindControl(_T("home"))->SetVisible(true);
		}
		else
		{
			if (m_pManager->FindControl(_T("home"))->IsVisible())
			{
				m_pManager->FindControl(_T("home"))->SetVisible(false);
			}
			static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_menuitem")))->SelectItem(StrToInt(GetUserData()));
		}
	}
	else if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}

void CPopupMenuUI::SetMenuBkColor(DWORD menuColor, DWORD itemColor)
{
	CContainerUI *layout;
	layout = (CContainerUI*)GetParent();
	layout->SetBkColor(menuColor);
	for (int i = 0; i < layout->GetCount(); i+=2)
		layout->GetItemAt(i)->SetBkColor(menuColor);

	SetBkColor(0xFF47688F);
	SetTextColor(0xFFFFFFFF);

	CControlUI *pItem;
	pItem =m_pManager->GetFocus();
	layout = (CContainerUI*)pItem->GetParent();
	layout->SetBkColor(itemColor);
	for (int i = 0; i < layout->GetCount(); i += 2) {
		if(layout->GetItemAt(i)!= pItem)
			layout->GetItemAt(i)->SetBkColor(itemColor);
	}
	
}

void CPopupMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CButtonUI::SetAttribute(pstrName, pstrValue);
	if (_tcsicmp(pstrName, _T("ismark")) == 0) 
		SetMark(pstrValue);
}

void CPopupMenuUI::SetMark(LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrValue, _T("true")) == 0)
		isMark = true;
	else isMark = false;
}

void CPopupMenuUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);
	CDuiString text;
	CPort *pPort = (CPort*)GetTag();

	if (pPort) {
		mHintNumber = pPort->m_virginNumber;
	}

	if (mHintNumber<99)
		text.Format(_T("%d"), mHintNumber);
	else
		text = _T("99");

	if (mHintNumber != 0 && isMark==true) {
		RECT rcPos = {259, 10, 294, 45};
		RECT textPos = GetPos();

		CDuiString dest;

		if (mHintNumber>9) {
			textPos.left += 263;
			textPos.top += 17;
			textPos.right = textPos.left + 290;
			textPos.bottom = textPos.top + 38;
		}
		else {
			textPos.left += 270;
			textPos.top += 16;
			textPos.right = textPos.left + 291;
			textPos.bottom = textPos.top + 41;
		}

		dest.Format(_T("file='image\\椭圆形.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
		CRenderEngine::DrawText(hDC, m_pManager, textPos, text, 0xFFFFFF, 1, 0);
	}
}


void CPopupMenuUI::SetHintNum(int num)
{
}

int CPopupMenuUI::GetHintNum()
{
	return mHintNumber;
}

