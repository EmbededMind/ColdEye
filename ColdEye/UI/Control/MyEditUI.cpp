#include "stdafx.h"
#include "MyEditUI.h"
#include "Wnd/MsgWnd.h"

IMPLEMENT_DUICONTROL(CMyEditUI)
CMyEditUI::CMyEditUI()
{
}


CMyEditUI::~CMyEditUI()
{
}


void CMyEditUI::DoEvent(TEventUI &event)
{
	if (event.Type == UIEVENT_KEYDOWN){
		CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
		if (event.wParam == VK_RETURN){
			if (GetName().CompareNoCase(_T("edit_shipname")) == 0){
				CControlUI *pCtl =m_pManager->FindControl(_T("keyboard"));
				if (!pCtl->IsVisible()){
					pCtl->SetVisible(true);
					m_pManager->FindControl(_T("prompt"))->SetVisible(true); //提示信息
					m_pManager->SetFocus(m_pManager->FindControl(_T("vk_1")));
				}
			}
			if (GetName().CompareNoCase(_T("cameraset_shipname")) == 0) {//摄像头设置
				CVerticalLayoutUI *pLayout;
				CVerticalLayoutUI *pChildLayout1;
				CVerticalLayoutUI *pChildLayout2;
				CContainerUI *pChildItem;

				CRect rect ;
				CSize size;
				pLayout = (CVerticalLayoutUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());

				pChildLayout1 = (CVerticalLayoutUI*)pLayout->GetItemAt(1);
				pChildLayout2 = (CVerticalLayoutUI*)pLayout->GetItemAt(2);
				if (pChildLayout1->IsVisible() == false){
					rect = pChildLayout2->GetPos();
					size.cx =  70;
					size.cy = rect.top + 204;
					pChildLayout2->SetFixedXY(size);
					pChildLayout1->SetVisible(true);
					pChildItem = (CContainerUI*)pChildLayout1->GetItemAt(0);
					pChildItem = (CContainerUI*)pChildItem->GetItemAt(0);
					pChildItem = (CContainerUI*)pChildItem->GetItemAt(0);
					pChildItem->SetFocus();
				}
			}
		}

		if (event.wParam == VK_DOWN){
			if (GetName() == _T("cameraset_shipname")){
				CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pTabLayout->GetItemAt(pTabLayout->GetCurSel());
				CVerticalLayoutUI *pBottomLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //最底下布局
				pBottomLayout->GetItemAt(1)->SetFocus();
			}
		}

		if (event.wParam==VK_BACK) {
			if (GetName() == _T("edit_shipname")) {
				m_pManager->FindControl(_T("mastername"))->SetFocus();
			}
			else if (GetName() == _T("cameraset_shipname")) {
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"));
				int index;
				CTabLayoutUI *ChildLayout;
				index = pTabLayout->GetItemIndex(GetParent()->GetParent());
				index -= 6;
				ChildLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_submenu_setting")));
				ChildLayout->GetItemAt(index * 2)->SetFocus();
			}
		}
		

	}


	if (event.Type == UIEVENT_SETFOCUS){
		SetBkColor(0xFFD7E9FF);
	}

	CButtonUI::DoEvent(event);
}