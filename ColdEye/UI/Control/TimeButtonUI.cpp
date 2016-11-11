#include "stdafx.h"

#include "TimeButtonUI.h"

IMPLEMENT_DUICONTROL(CTimeButtonUI)

CTimeButtonUI::CTimeButtonUI()
{}

CTimeButtonUI::~CTimeButtonUI()
{}


LPCTSTR CTimeButtonUI::GetClass() const
{
	return _T("TimeButtonUI");
}



void CTimeButtonUI::SetItemRelation(CTimeButtonUI * pPrevItem, CTimeButtonUI * pNextItem)
{
	mPrevItem = pPrevItem;
	mNextItem = pNextItem;
}

void CTimeButtonUI::DecreaseHour()
{
	CControlUI* pBtn = m_pManager->GetFocus();

	int val;
	CDuiString text = pBtn->GetText();
	val = StrToInt(text);

	if (val > 0) {
		val--;

		text.Format(_T("%02d"), val);
	}
	else {
		//val = 23;
		//text.Format(_T("%d"), val);
		text.Format(_T("23"), val);
	}

	pBtn->SetText(text);
}

void CTimeButtonUI::DecreaseMinute()
{
	CControlUI* pBtn = m_pManager->GetFocus();

	int val;
	CDuiString text = pBtn->GetText();
	val = StrToInt(text);
	if (val > 0) {
		val--;
	}
	else {
		val = 59;
	}

	text.Format(_T("%02d"), val);
	pBtn->SetText(text);
}

void CTimeButtonUI::IncreaseHour()
{
	CControlUI* pBtn = m_pManager->GetFocus();
	int val;
	CDuiString text = pBtn->GetText();
	val = StrToInt(text);

	if (val < 23) {
		val++;
	}
	else {
		val = 0;
	}

	text.Format(_T("%02d"), val);
	pBtn->SetText(text);
}

void CTimeButtonUI::IncreaseMinute()
{
	CControlUI* pBtn = m_pManager->GetFocus();
}


void CTimeButtonUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN) {
		switch (event.wParam)
		{
			case VK_UP:
				if (event.pSender == m_pManager->FindControl(_T("time1_hour"))
					|| event.pSender == m_pManager->FindControl(_T("time2_hour"))) {
					DecreaseHour();
				}
				else {
					DecreaseMinute();
				}
				break;
			//---------------------------------------------------
			case VK_DOWN:
				if (event.pSender == m_pManager->FindControl(_T("time1_hour"))
					|| event.pSender == m_pManager->FindControl(_T("time2_hour"))) {
					IncreaseHour();
				}
				else {
					IncreaseMinute();
				}
				break;
			//---------------------------------------------------
			case VK_LEFT:
				if (mPrevItem != NULL) {
					mPrevItem->SetFocus();
				}
				break;
			//---------------------------------------------------
			case VK_RIGHT:
				if (mNextItem != NULL) {
					mNextItem->SetFocus();
				}
				break;
			//---------------------------------------------------
			case VK_RETURN:
				
				break;
		}
	}
     
	if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFF3FF3F3);
	}

	CButtonUI::DoEvent(event);
}