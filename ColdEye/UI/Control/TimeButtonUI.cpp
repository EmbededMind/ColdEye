#include "stdafx.h"

#include "TimeButtonUI.h"
#include "Wnd/MsgWnd.h"

#include "Pattern\MsgSquare.h"

#include "ColdEye.h"

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
	GetTime();
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
	GetTime();
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
	GetTime();
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
	GetTime();
}


bool CTimeButtonUI::isMorrow()
{
	if (tMinute_1_New <= tMinute_2_New)
		return false;
	else
		return true;
}


void CTimeButtonUI::GetTime()
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
	tMinute_1_New = tHour1 * 60 + tMinute1;
	tMinute_2_New = tHour2 * 60 + tMinute2;
}



void CTimeButtonUI::SetMinutes(DWORD tBegining, DWORD tEnd)
{
	tMinute_1  = tBegining;
	tMinute_2  = tEnd;
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
				if (tMinute_1_New != tMinute_1 || tMinute_2_New != tMinute_2) {

					tMinute_1 = tMinute_1_New;
					tMinute_2 = tMinute_2_New;
					//CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"));
					
					//if IDOK
					if (true) {
						if (((CColdEyeApp*)AfxGetApp())->SetAwTime(tMinute_1, tMinute_2)) {
							MSG msg;
							msg.message = USER_MSG_CAMERA_CONFIG_AWTIME;
/*							msg.wParam = tMinute_1;
							msg.lParam = tMinute_2*/;
							CMsgSquare::GetInstance()->Broadcast(msg);
						}
					}
				}
				m_pManager->FindControl(_T("watchtime"))->SetFocus();
				break;
		}
	}
     
	
	if (event.Type == UIEVENT_KILLFOCUS) {
		SetBkColor(0xFFF3F3F3);
	}

	CButtonUI::DoEvent(event);
}