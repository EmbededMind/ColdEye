#include "stdafx.h"
#include "MyEditUI.h"
#include "conio.h"
#include "CameraSwitchUI.h"

IMPLEMENT_DUICONTROL(CMyEditUI)
CMyEditUI::CMyEditUI()
{
}


CMyEditUI::~CMyEditUI()
{
}


void CMyEditUI::DoEvent(TEventUI &event)
{
	if (event.Type == UIEVENT_KEYDOWN)
	{
		if (event.wParam == VK_RETURN)
		{
			if (GetName().CompareNoCase(_T("shipname")) == 0)
			{
				CControlUI *pCtl = static_cast<CControlUI*>(m_pManager->FindControl(_T("keyboard")));
				if (!pCtl->IsVisible())
				{
					pCtl->SetVisible(true);
					static_cast<CControlUI*>(m_pManager->FindControl(_T("prompt")))->SetVisible(true); //提示信息
					m_pManager->SetFocus(static_cast<CButtonUI*>(m_pManager->FindControl(_T("vk_1"))));
				}
			}

			if (GetName().CompareNoCase(_T("cameraset")) == 0) //摄像头设置
			{
				CTabLayoutUI *pParTabLayout;
				CVerticalLayoutUI *pParVerLayout;
				CTabLayoutUI *pChildLayout1;
				CTabLayoutUI *pChildLayout2;

				CRect rect ;
				CSize size;
				pParTabLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				pParVerLayout = (CVerticalLayoutUI*)pParTabLayout->GetItemAt(pParTabLayout->GetCurSel());

				pChildLayout1 = (CTabLayoutUI*)pParVerLayout->GetItemAt(1);
				pChildLayout2 = (CTabLayoutUI*)pParVerLayout->GetItemAt(2);
				if (pChildLayout1->IsVisible() == false)
				{
					rect = pChildLayout2->GetPos();
					size.cx =  70;
					size.cy = rect.top + 204;
					pChildLayout2->SetFixedXY(size);
					pChildLayout1->SetVisible(true);
				}
				else
				{
					pChildLayout1->SetVisible(false);
					rect = pChildLayout2->GetPos();
					size.cx = 70;
					size.cy = rect.top - 204;
					pChildLayout2->SetFixedXY(size);
				}
			}
		}

		if (event.wParam == VK_DOWN)
		{
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
			CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTab->GetItemAt(pTab->GetCurSel());
			CVerticalLayoutUI *pBottomLayout =  (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //最底下布局
			static_cast<CCameraSwitchUI*> (pBottomLayout->GetItemAt(1))->SetFocus();
		}
	}


	if (event.Type == UIEVENT_SETFOCUS)
	{
		_cprintf("focus\n");
		SetBkColor(0xFFD7E9FF);
	}

	CButtonUI::DoEvent(event);
}