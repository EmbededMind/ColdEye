#include "stdafx.h"
#include "MyLabelUI.h"

IMPLEMENT_DUICONTROL(CMyLabelUI)
CMyLabelUI::CMyLabelUI()
	:m_stateEnableText(_T("")),
	m_stateDisableText(_T("")),
	Value(false)
{
}


CMyLabelUI::~CMyLabelUI()
{
}


void CMyLabelUI::PaintText(HDC hDC)
{
	CButtonUI::PaintText(hDC);
	RECT rc = m_rcItem;
	RECT m_rcTextPadding = CLabelUI::m_rcTextPadding;
	UINT TextStyle;
	if (Value)
	{
		if (m_stateEnableText.IsEmpty())return;

		rc.right -= 45;
		TextStyle = m_uTextStyle;
		TextStyle &= ~(DT_LEFT | DT_CENTER);
		TextStyle |= DT_RIGHT;
		if (m_bFocused)
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_stateEnableText, m_stateTextFocusColor, \
				m_iFont, TextStyle);
		}
		else
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_stateEnableText, m_stateTextColor, \
				m_iFont, TextStyle);
		}
	}
	else
	{
		if (m_stateDisableText.IsEmpty())return;

		rc.right -= 45;
		TextStyle = m_uTextStyle;
		TextStyle &= ~(DT_LEFT | DT_CENTER);
		TextStyle |= DT_RIGHT;
		if (m_bFocused)
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_stateDisableText, m_stateTextFocusColor, \
				m_iFont, TextStyle);
		}
		else
		{
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_stateDisableText, m_stateTextColor, \
				m_iFont, TextStyle);
		}
	}
}

void CMyLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("stateenabletext")) == 0)
		SetStateEnableText(pstrValue);
	else if (_tcsicmp(pstrName, _T("statedisabletext")) == 0)
		SetStateDisableText(pstrValue);
	else if (_tcsicmp(pstrName, _T("statetextcolor")) == 0)
	{
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetStateTextColor(clrColor);
	}
	else if (_tcsicmp(pstrName, _T("statetextfocuscolor")) == 0)
	{
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetStateTextFocusColor(clrColor);
	}
	else if (_tcsicmp(pstrName, _T("focusbkcolor")) == 0)
	{
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFocusBkColor(clrColor);
	}
	else if (_tcsicmp(pstrName, _T("selbkimage")) == 0)
	{
		SetSelBkImage(pstrValue);
	}
	else if (_tcsicmp(pstrName, _T("focusedselbkimage")) == 0)
	{
		SetFocusedSelBkImage(pstrValue);
	}
	CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CMyLabelUI::SetStateEnableText(LPCTSTR pstrText)
{
	m_stateEnableText = pstrText;
}

void CMyLabelUI::SetStateDisableText(LPCTSTR pstrText)
{
	m_stateDisableText = pstrText;
}

void CMyLabelUI::SetStateTextColor(DWORD Color)
{
	m_stateTextColor = Color;
}

void CMyLabelUI::SetStateTextFocusColor(DWORD Color)
{
	m_stateTextFocusColor = Color;
}

void CMyLabelUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);
	if (Value)
	{
		if (m_bFocused)
		{
			if (!m_focusedselbkimage.IsEmpty())
				DrawImage(hDC, (LPCTSTR)m_focusedselbkimage);
		}
		else
		{
			if(!m_selbkimage.IsEmpty())
				DrawImage(hDC, (LPCTSTR)m_selbkimage);
		}
	}
}

void CMyLabelUI::PaintBkColor(HDC hDC)
{

	if (!m_bFocused)
	{
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
	}
	else
	{
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_focusBkColor));
	}
}

void CMyLabelUI::SetFocusBkColor(DWORD Color)
{
	m_focusBkColor = Color;
}

void CMyLabelUI::SetFocusedSelBkImage(LPCTSTR pstrValue)
{
	m_focusedselbkimage = pstrValue;
}

void CMyLabelUI::SetSelBkImage(LPCTSTR pstrValue)
{
	m_selbkimage = pstrValue;
}

void CMyLabelUI::DoEvent(TEventUI & event)
{
	switch (event.Type)
	{
	case UIEVENT_KEYDOWN:{
		CVerticalLayoutUI *pParentLayout = (CVerticalLayoutUI*)GetParent();
		switch (event.wParam) {
			case VK_UP:
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) - 2)->SetFocus();
				break;

			case VK_DOWN:
				if (GetName() == _T("camera_set_video_save"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();
				else if (GetName() == _T("sysset_version"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//向下移动2
				else if (GetName() == _T("sysset_reset"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//向下移动2
				else if (GetName() == _T("sysset_host_model"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//向下移动2
				break;

			case VK_LEFT:
				break;

			case VK_RIGHT:
				break;
			}
		}
		break;
	default:
		CButtonUI::DoEvent(event);
		break;
	}
}

