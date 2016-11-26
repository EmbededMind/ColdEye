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
		switch (event.wParam)
		{
		case VK_UP:
			if (line > 0)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line - 1);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row));
			}
			break;

		case VK_DOWN:
			if (line < 3)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line + 1);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row));
			}
			break;

		case VK_LEFT:
			if (row > 0)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row - 1));
			}
			break;

		case VK_RIGHT:
			if (line<2 && row<5)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row + 1));
			}
			else if (line == 2 && row < 4)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row + 1));
			}
			break;

		//case VK_RETURN:
		//	CMyEditUI * pItem;
		//	pLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
		//	pLayout = (CVerticalLayoutUI*)pLayout->GetItemAt(0);
		//	pItem = (CMyEditUI*)pLayout->GetItemAt(2);
		//	pItem->SetText(GetText());
		//	break;
		//case VK_BACK:
		//	{
		//		CTabLayoutUI *pParTabLayout;
		//		CVerticalLayoutUI *pParVerLayout;
		//		CTabLayoutUI *pChildLayout1;
		//		CTabLayoutUI *pChildLayout2;

		//		CRect rect;
		//		CSize size;
		//		pParTabLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
		//		pParVerLayout = (CVerticalLayoutUI*)pParTabLayout->GetItemAt(pParTabLayout->GetCurSel());

		//		pChildLayout1 = (CTabLayoutUI*)pParVerLayout->GetItemAt(1);
		//		pChildLayout2 = (CTabLayoutUI*)pParVerLayout->GetItemAt(2);
		//		if (pChildLayout1->IsVisible() == true)
		//		{
		//			pChildLayout1->SetVisible(false);
		//			rect = pChildLayout2->GetPos();
		//			size.cx = 70;
		//			size.cy = rect.top - 204;
		//			pChildLayout2->SetFixedXY(size);
		//			CVerticalLayoutUI *pLayout = (CVerticalLayoutUI*)(GetParent()->GetParent()->GetParent()->GetParent());
		//			CMyEditUI *pItem = (CMyEditUI*)(static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(0))->GetItemAt(2));
		//			//pItem->isEditing = false;
		//			//pItem->SetFocus();
		//		}

		//	}
		//	break;
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
