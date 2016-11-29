#include "stdafx.h"
#include "RevordVoiceBtUI.h"
#include "Control\AlarmVoiceListUI.h"
#include "Wnd\MsgWnd.h"

IMPLEMENT_DUICONTROL(CRevordVoiceBtUI)
CRevordVoiceBtUI::CRevordVoiceBtUI()
{
}


CRevordVoiceBtUI::~CRevordVoiceBtUI()
{
}

void CRevordVoiceBtUI::DoEvent(TEventUI & event)
{
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		m_pManager->SendNotify(this, DUI_MSGTYPE_RECORDVOICE, NULL, NULL);
		break;
	}
	CButtonUI::DoEvent(event);
}
