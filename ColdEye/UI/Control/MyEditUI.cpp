#include "stdafx.h"
#include "MyEditUI.h"
#include "Wnd/MsgWnd.h"

IMPLEMENT_DUICONTROL(CMyEditUI)
CMyEditUI::CMyEditUI()
	:isEditing(false)
{
}


CMyEditUI::~CMyEditUI()
{
}


void CMyEditUI::DoEvent(TEventUI &event)
{
	if (event.Type == UIEVENT_KEYDOWN){
		m_pManager->SendNotify(this, DUI_MSGTYPE_EDIT,event.wParam,event.lParam);
	}


	if (event.Type == UIEVENT_SETFOCUS){
		SetBkColor(0xFFD7E9FF);
	}
	else if (event.Type == UIEVENT_KILLFOCUS) {
		if(isEditing==false)
			SetBkColor(0xFFEBEBEB);
	}

	CButtonUI::DoEvent(event);
}

void CMyEditUI::SetName(CString name)
{
	SetUserData(name);
	SetText(name);
}

void CMyEditUI::SetStatus(bool newStatus)
{
	isEditing = newStatus;
}

bool CMyEditUI::GetStatus()
{
	return isEditing;
}
