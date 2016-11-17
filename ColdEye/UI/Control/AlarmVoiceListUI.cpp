#include "stdafx.h"
//#include "AlarmVoiceListUI.h"
#include "Control\AlarmVoiceListUI.h"

IMPLEMENT_DUICONTROL(CAlarmVoiceListUI)
CAlarmVoiceListUI::CAlarmVoiceListUI()
	:state(false)
{
}

CAlarmVoiceListUI::CAlarmVoiceListUI(CDuiString text, CDuiString name)
	: state(false)
{
	SetText(text);
	SetName(name);
}

CAlarmVoiceListUI::~CAlarmVoiceListUI()
{
}



void CAlarmVoiceListUI::PaintBkColor(HDC hDC)
{
	if (m_bFocused)
	{
		m_dwBackColor = 0xFF4198FE;
	}
	else
	{
		m_dwBackColor = 0xFFF3F3F3;
	}
	CButtonUI::PaintBkColor(hDC);
}

void CAlarmVoiceListUI::PaintStatusImage(HDC hDC)
{
	m_rc = m_rcPaint;
	m_rc.left += 44;
	m_rc.right -= 46;
	m_rc.top += 2;
	m_rc.bottom -= 2;
	CRenderEngine::DrawColor(hDC, m_rc, GetAdjustColor(0xFFBFDDFF));
	if (state)
	{
		DrawImage(hDC, (LPCTSTR)m_sPushedImage);
	}
	else
	{
		DrawImage(hDC, (LPCTSTR)m_sNormalImage);
	}
}

void CAlarmVoiceListUI::DoEvent(TEventUI & event)
{
	switch (event.Type){
		case UIEVENT_KEYDOWN:{
			switch (event.wParam){
			case VK_UP:
				m_pManager->FindControl(_T("alarmvoice_switch"))->SetFocus();
				break;

			case VK_DOWN:
				m_pManager->FindControl(_T("record_btn"))->SetFocus();
				break;

			case VK_RETURN:
				break;
			}
		}
		break;
		default:
			CButtonUI::DoEvent(event);
			break;
	}

}
