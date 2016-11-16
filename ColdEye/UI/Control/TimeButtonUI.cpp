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
	int val;
	CDuiString text = GetText();
	val = StrToInt(text);

	if (val > 0) {
		val--;

	}
	else {
		val = 23;
	}
	text.Format(_T("%02d"), val);
	SetText(text);
	SetTime();
}

void CTimeButtonUI::DecreaseMinute()
{
	int val;
	CDuiString text = GetText();
	val = StrToInt(text);
	if (val > 0) {
		val--;
	}
	else {
		val = 59;
	}

	text.Format(_T("%02d"), val);
	SetText(text);
	SetTime();
}

void CTimeButtonUI::IncreaseHour()
{
	int val;
	CDuiString text = GetText();
	val = StrToInt(text);

	if (val < 23) {
		val++;
	}
	else {
		val = 0;
	}

	text.Format(_T("%02d"), val);
	SetText(text);
	SetTime();
}

void CTimeButtonUI::IncreaseMinute()
{
	int val;
	CDuiString text = GetText();
	val = StrToInt(text);
	if (val <59) {
		val++;
	}
	else {
		val = 0;
	}

	text.Format(_T("%02d"), val);
	SetText(text);
	SetTime();
}

bool CTimeButtonUI::isMorrow()
{
	if (tMinute_1 <= tMinute_2)
		return false;
	else
		return true;
}

void CTimeButtonUI::SetTime()
{
	CButtonUI *pHour1, *pHour2, *pMinute1, *pMinute2;
	int tHour1, tHour2, tMinute1, tMinute2;
	pHour1 = (CButtonUI*)m_pManager->FindControl(_T("time1_hour"));
	pHour2 = (CButtonUI*)m_pManager->FindControl(_T("time2_hour"));
	pMinute1 = (CButtonUI*)m_pManager->FindControl(_T("time1_minute"));
	pMinute2 = (CButtonUI*)m_pManager->FindControl(_T("time2_minute"));
	tHour1 = StrToInt(pHour1->GetText());
	tHour2 = StrToInt(pHour2->GetText());
	tMinute1 = StrToInt(pMinute1->GetText());
	tMinute2 = StrToInt(pMinute2->GetText());
	tMinute_1 = tHour1 * 60 + tMinute1;
	tMinute_2 = tHour2 * 60 + tMinute2;

}


void CTimeButtonUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN) {

		CButtonUI* pButton = (CButtonUI*)m_pManager->FindControl(_T("morrow"));
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
				if (isMorrow()) {
					if (!pButton->IsVisible())
						pButton->SetVisible(true);
				}
				else if(pButton->IsVisible()) {
					pButton->SetVisible(false);
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
				if (isMorrow()) {
					if (!pButton->IsVisible())
						pButton->SetVisible(true);
				}
				else if (pButton->IsVisible()) {
					pButton->SetVisible(false);
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
			case VK_BACK:
				m_pManager->FindControl(_T("watchtime"))->SetFocus();
				break;
		}
	}
     
	
	if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFF3F3F3);
	}

	CButtonUI::DoEvent(event);
}