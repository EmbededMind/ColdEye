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
		//----------------------------------------------------------------------
		case VK_LEFT:
			SetValue(GetValue()-1);
			Print("value:%d", GetValue());
			m_pManager->SendNotify(this, DUI_MSGTYPE_VALUECHANGED, (WPARAM)GetValue(),NULL);
			break;
		//----------------------------------------------------------------------
		case VK_RIGHT:
			SetValue(GetValue() + 1);
			m_pManager->SendNotify(this, DUI_MSGTYPE_VALUECHANGED, (WPARAM)GetValue(), NULL);
			break;
		//----------------------------------------------------------------------
		}
	}
	else if (event.Type == UIEVENT_SETFOCUS) {
		CDuiString name;
		name = bindControl->GetName();
		Print("Biu----name:%S", name);
		bindControl->SetBkImage(m_focusedbkimage);
		bindControl->Invalidate();
	}
	else if (event.Type == UIEVENT_KILLFOCUS) {
		bindControl->SetBkImage(m_bkimage);
		bindControl->Invalidate();
	}
	CSliderUI::DoEvent(event);
}

void CMySliderUI::SetAttribute(LPCTSTR strName, LPCTSTR strValue)
{
	if (_tcsicmp(strName, _T("focusedbkimage")) == 0)
	{
		m_focusedbkimage = strValue;
	}
	else if(_tcsicmp(strName, _T("bindbkimage")) == 0)
	{
		m_bkimage = strValue;
	}
	__super::SetAttribute(strName, strValue);
}

//void CMySliderUI::PaintBkColor(HDC hDC)
//{	
//	CRect m_rc;
//	if (m_bFocused)
//	{
//		m_dwBackColor = 0xFF4198FE;
//		m_rc = m_rcPaint;
//		m_rc.left -= 176;
//		m_rc.top -= 28;
//		m_rc.bottom += 28;
//		m_rc.right += 184;
//		CRenderEngine::DrawColor(hDC, m_rc, GetAdjustColor(m_dwBackColor));
//	}
//	else
//	{
//		m_dwBackColor = 0xFFF3F3F3;
//		CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
//	}
//	InvalidateRect(m_pManager->GetPaintWindow(),m_rc,true);
//}
//
//void CMySliderUI::PaintBkImage(HDC hDC)
//{
//	CRect m_rc;
//	m_rc = m_rcPaint;
//	m_rc.left -= 176;
//	m_rc.top -= 28;
//	m_rc.bottom += 28;
//	m_rc.right += 184;
//	CRenderEngine::DrawImageString(hDC, m_pManager, m_rc, m_rc, (LPCTSTR)m_sBkImage, NULL, NULL);
//	Invalidate();
//}
//
