#include "stdafx.h"

#include "TimeButtonUI.h"
#include "Wnd/MsgWnd.h"

#include "Pattern\MsgSquare.h"

#include "ColdEye.h"

IMPLEMENT_DUICONTROL(CTimeButtonUI)

CTimeButtonUI::CTimeButtonUI()
	:m_Value(0)
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

	SetValue(val*60);

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

	SetValue(val);

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

	SetValue(val*60);

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

	SetValue(val);

}


bool CTimeButtonUI::isMorrow()
{
	CTimeButtonUI *pHour1, *pHour2, *pMinute1, *pMinute2;
	DWORD tMinute1, tMinute2;
	pHour1 = (CTimeButtonUI*)m_pManager->FindControl(_T("time1_hour"));
	pHour2 = (CTimeButtonUI*)m_pManager->FindControl(_T("time2_hour"));
	pMinute1 = (CTimeButtonUI*)m_pManager->FindControl(_T("time1_minute"));
	pMinute2 = (CTimeButtonUI*)m_pManager->FindControl(_T("time2_minute"));


	tMinute1 = pHour1->GetValue() + pMinute1->GetValue();
	tMinute2 = pHour2->GetValue() + pMinute2->GetValue();

	if (tMinute1 <= tMinute2)
		return false;
	else
		return true;
}




void CTimeButtonUI::SetValue(int value)
{
	CString text;
	m_Value = value;
	text.Format(_T("%02d"), m_Value);
	SetText(text);

}

int CTimeButtonUI::GetValue()
{
	return m_Value;
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

//					if (true) {

//					}

                SendMessage(m_pManager->GetPaintWindow(), USER_MSG_CAMERA_CONFIG_AWTIME, 0, 0);
	
				m_pManager->FindControl(_T("watchtime"))->SetFocus();
				break;
		}
	}
     
	
	if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFF3F3F3);
	}

	CButtonUI::DoEvent(event);
}