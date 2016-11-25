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
		case VK_DOWN:
			m_pManager->SendNotify(this, DUI_MSGTYPE_SLIDER, event.wParam, event.lParam);
			break;
		case VK_BACK: //返回当前的值
			m_pManager->SendNotify(this, DUI_MSGTYPE_SLIDER, event.wParam,GetValue());
			break;

		case VK_LEFT:
			SetValue(GetValue()-1);
			break;

		case VK_RIGHT:
			SetValue(GetValue() + 1);
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
