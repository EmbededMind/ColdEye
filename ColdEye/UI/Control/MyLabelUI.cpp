#include "stdafx.h"
#include "MyLabelUI.h"
#include "Wnd\MsgWnd.h"

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
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//�����ƶ�2
				else if (GetName() == _T("sysset_reset"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//�����ƶ�2
				else if (GetName() == _T("sysset_host_model"))
					pParentLayout->GetItemAt(pParentLayout->GetItemIndex(this) + 2)->SetFocus();//�����ƶ�2
				break;

			case VK_LEFT:
				StatusUpdate();
				break;

			case VK_RIGHT:
				StatusUpdate();
				break;
			}
		}
		break;
	default:
		CButtonUI::DoEvent(event);
		break;
	}

	//CButtonUI::DoEvent(event);
}

void CMyLabelUI::StatusUpdate()
{
	if (GetName() == _T("camera_set_video_save")\
		|| GetName() == _T("camera_set_shipwatch")) {
		if (Value) {
			if (GetName() == _T("camera_set_video_save")) {
				if(MSGID_OK == CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_okcancel.xml"), \
					_T("�رմ����������Ƶ�������޷���"), _T("����Ƶ���Ƿ�ȷ���رգ�")))
					Value = false;
			}
			else {
				if(MSGID_OK == CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_okcancel.xml"), \
					_T("�ر�������Զ������󣬸�����ͷ��"), _T("���ᷢ���������Ƿ�ȷ���رգ�")))
					Value = false;
			}
		}
		else {
			Value = true;
		}
		Invalidate();
	}
	else {
		if (GetName() == _T("sysset_version")) {
			int i = 3;
			if(i==0)
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_ok.xml"), NULL, _T("δ��⵽U�̣������ԣ�"));
			else if (i == 1) {
				if (MSGID_OK == CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_update_request.xml"), NULL, NULL)) {
					CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_update.xml"), _T("V2.0.0"), NULL);
					CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_update_success.xml"), _T("����汾��V2.0.0"), NULL);
				}
			}
			else if(i==2)
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_ok.xml"), _T("U��δ���ָ��İ汾��������³�"), _T("��"));
			else if(i==3)
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_update.xml"), _T("V2.0.0"),NULL);
			else if(i==4)
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_update_success.xml"), _T("����汾��V2.0.0"), NULL);
		}
		else if (GetName() == _T("sysset_reset")) {
			CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_okcancel.xml"), NULL, _T("ȷ���ָ��������ã�"));
		}
	}
}

