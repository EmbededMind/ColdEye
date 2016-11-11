#include "stdafx.h"
#include "MySliderUI.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CMySliderUI)
CMySliderUI::CMySliderUI()
{
	//pSlider = new CSliderUI;
}


CMySliderUI::~CMySliderUI()
{
}

void CMySliderUI::DoEvent(TEventUI & event)
{
	if (event.Type == UIEVENT_KEYDOWN)
	{
		switch (event.wParam)
		{
		case VK_UP:
			break;

		case VK_DOWN:
			break;

		case VK_LEFT:
			SetValue(GetValue()-1);
			break;

		case VK_RIGHT:
			SetValue(GetValue() + 1);
			break;
		}
		_cprintf("v = %d\n",GetValue());

	}
	CSliderUI::DoEvent(event);
}

void CMySliderUI::PaintBkColor(HDC hDC)
{
	if (m_bFocused)
	{
		m_dwBackColor = 0xFF4198FE;
		m_rc = m_rcPaint;
		m_rc.left -= 176;
		m_rc.top -= 24;
		m_rc.bottom += 24;
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
	m_rc = m_rcPaint;
	m_rc.left -= 176;
	m_rc.top -= 24;
	m_rc.bottom += 24;
	m_rc.right += 184;
	CRenderEngine::DrawImageString(hDC, m_pManager, m_rc, m_rc, (LPCTSTR)m_sBkImage, NULL, NULL);
	Invalidate();
}
