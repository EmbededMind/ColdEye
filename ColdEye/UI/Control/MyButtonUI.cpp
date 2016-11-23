#include "stdafx.h"
#include "MyButtonUI.h"
#include "Control\AlarmVoiceListUI.h"
#include "Wnd\MsgWnd.h"

IMPLEMENT_DUICONTROL(CMyButtonUI)
CMyButtonUI::CMyButtonUI()
{
}


CMyButtonUI::~CMyButtonUI()
{
}

void CMyButtonUI::DoEvent(TEventUI & event)
{
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		switch (event.wParam)
		{
		case VK_UP:
			if (GetName() == _T("record_btn")) {
				m_pManager->FindControl(_T("alarmvoice_switch"))->SetFocus();
			}
			break;

		case VK_RETURN:
			{
				SendMessage(m_pManager->GetPaintWindow(),USER_MSG_MESSAGE_BOX, RECORD, NULL);
				//CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_recordingvoice.xml"), NULL, NULL);
				//CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_playvoice.xml"), NULL, NULL);
				CContainerUI *layout;
				SIZE size_bnt = GetFixedXY();
				CAlarmVoiceListUI *pVoice = new CAlarmVoiceListUI(_T("录音1"),_T("voice1"));
				size_bnt.cy += 64;
				layout = (CContainerUI*)GetParent();
				layout->SetFixedHeight(layout->GetHeight()+74);
				SetFixedXY(size_bnt);
				SetText(_T("重新录制报警音"));
				layout->Add(pVoice);
				pVoice->SetAttribute(_T("style"), _T("alarm_voice_list"));
				pVoice->SetFixedXY({ 0,360 });
			}
			break;

		case VK_LEFT:
			SendMessage(m_pManager->GetPaintWindow(), USER_MSG_MESSAGE_BOX, RECORD, NULL);
			break;
		}
		break;
	}
	CButtonUI::DoEvent(event);
}
