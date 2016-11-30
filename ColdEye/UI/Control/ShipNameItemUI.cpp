#include "stdafx.h"
#include "ShipNameItemUI.h"
#include "MyEditUI.h"

IMPLEMENT_DUICONTROL(CShipNameItemUI)
CShipNameItemUI::CShipNameItemUI()
{
}


CShipNameItemUI::~CShipNameItemUI()
{
}


LPCTSTR CShipNameItemUI::GetClass() const
{
	return _T("ShipNameItemUI");
}

void CShipNameItemUI::DoEvent(TEventUI &event)
{
	int userdata, line, row;
	CVerticalLayoutUI *pLayout;	//整体布局
	CHorizontalLayoutUI *pLineLayout; //行布局
	CTabLayoutUI *pTab;
	pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
	pLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
	pLayout = (CVerticalLayoutUI*)pLayout->GetItemAt(1);
	pLayout = (CVerticalLayoutUI*)pLayout->GetItemAt(0);
	if (event.Type == UIEVENT_KEYDOWN)
	{
		userdata = StrToInt(GetUserData());
		line = userdata / 10;
		row = userdata % 10;
		Print("line:%d, row:%d",line,row);
		switch (event.wParam)
		{
		//case VK_UP:
		//	if (line > 0)
		//	{
		//		pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line - 1);
		//		m_pManager->SetFocus(pLineLayout->GetItemAt(row));
		//	}
		//	break;

		//case VK_DOWN:
		//	if (line < 3)
		//	{

		//		pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line + 1);
		//		m_pManager->SetFocus(pLineLayout->GetItemAt(row));
		//	}
		//	break;

		case VK_LEFT:
			if (row > 0)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row - 1));
			}
			else if(line>0){
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line - 1);
				if (line==1 || line==2) {
					m_pManager->SetFocus(pLineLayout->GetItemAt(5));
				}
				else {
					m_pManager->SetFocus(pLineLayout->GetItemAt(4));
				}
			}
			break;

		case VK_RIGHT:
			if (line < 2) {
				if (row < 5) {
					pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
					m_pManager->SetFocus(pLineLayout->GetItemAt(row + 1));
				}
				else {
					pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line + 1);
					m_pManager->SetFocus(pLineLayout->GetItemAt(0));
				}
			}
			else if (line == 2) {
				if (row == 4) {
					pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line+1);
					m_pManager->SetFocus(pLineLayout->GetItemAt(0));
				}
				else {
					pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
					m_pManager->SetFocus(pLineLayout->GetItemAt(row + 1));
				}
			}
			break;

		case VK_RETURN:
			pLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
			pLayout = (CVerticalLayoutUI*)pLayout->GetItemAt(0);
			CButtonUI* pEdit = (CButtonUI*)pLayout->GetItemAt(2);
			int nameid = line*6+row+1+6;
			pEdit->SetTag(nameid);
			pEdit->SetText(GetText());
			break;
		}
	}

	if (event.Type == UIEVENT_SETFOCUS)
	{
		SetBkColor(0xFF4198FE);
	}
	else if (event.Type == UIEVENT_KILLFOCUS)
	{
		
		SetBkColor(0xFFD7E9FF);
	}
	CButtonUI::DoEvent(event);
}
