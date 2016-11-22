#include "stdafx.h"
#include "MySliderUI.h"

IMPLEMENT_DUICONTROL(CMySliderUI)
CMySliderUI::CMySliderUI()
{
}


CMySliderUI::~CMySliderUI()
{
}

void CMySliderUI::DoEvent(TEventUI & event)
{
	if (event.Type == UIEVENT_KEYDOWN)
	{
		CVerticalLayoutUI *pParentLayout = (CVerticalLayoutUI*)GetParent();
		switch (event.wParam)
		{
		case VK_UP:
			if (GetName() == _T("camera_set_volume") || GetName() == _T("sysset_voice"))
				pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) - 3)->SetFocus();//�����ƶ�3
			break;

		case VK_DOWN:
			if (GetName() == _T("sysset_light"))
				pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 3)->SetFocus();//�����ƶ�3
			else if(GetName() == _T("sysset_voice") || GetName() == _T("camera_set_volume"))
				pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//�����ƶ�2
			break;

		case VK_LEFT:
			SetValue(GetValue()-1);
			break;

		case VK_RIGHT:
			SetValue(GetValue() + 1);
			break;

		case VK_BACK:
			CTabLayoutUI *pLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
			BackPreviousItem(pLayout);
			break;
		}

	}
	CSliderUI::DoEvent(event);
}

void CMySliderUI::PaintBkColor(HDC hDC)
{	
	CRect m_rc;
	if (m_bFocused)
	{
		
		m_dwBackColor = 0xFF4198FE;
		m_rc = m_rcPaint;
		m_rc.left -= 176;
		m_rc.top -= 28;
		m_rc.bottom += 28;
		m_rc.right += 184;
		CRenderEngine::DrawColor(hDC, m_rc, GetAdjustColor(m_dwBackColor));
	}
	else
	{
		m_dwBackColor = 0xFFF3F3F3;
		CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
	}
	InvalidateRect(m_pManager->GetPaintWindow(),m_rc,true);
}

void CMySliderUI::PaintBkImage(HDC hDC)
{
	CRect m_rc;
	m_rc = m_rcPaint;
	m_rc.left -= 176;
	m_rc.top -= 28;
	m_rc.bottom += 28;
	m_rc.right += 184;
	CRenderEngine::DrawImageString(hDC, m_pManager, m_rc, m_rc, (LPCTSTR)m_sBkImage, NULL, NULL);
	Invalidate();
}

void CMySliderUI::BackPreviousItem(CTabLayoutUI* pParentLayout)
{
	int sel, count, focus_userdata;
	sel = pParentLayout->GetCurSel();
	CControlUI *pItem;
	CTabLayoutUI *pPrevLayout = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_submenu_setting")));
	count = pPrevLayout->GetCount();
	for (int i = 0; i < count; i += 2) {
		pItem = pPrevLayout->GetItemAt(i);
		focus_userdata = StrToInt(pItem->GetUserData());
		if (sel == focus_userdata) {
			pItem->SetFocus();
			break;
		}
	}
}
