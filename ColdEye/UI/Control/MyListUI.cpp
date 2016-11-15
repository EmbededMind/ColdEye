#include "stdafx.h"
#include "MyListUI.h"
#include "VideoListUI.h"

IMPLEMENT_DUICONTROL(CMyListUI)
CMyListUI::CMyListUI()
	:mIsLocked(false),
	prereadMessageNum(0),
	Info(0)
{
}


CMyListUI::CMyListUI(CRecordFileInfo* pInfo)
{
	Info = pInfo;
}

CMyListUI::~CMyListUI()
{
}

void CMyListUI::DoPaint(HDC hDC, const RECT & rcPaint)
{
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
	PaintBkColor(hDC);
	DrawItemBk(hDC, m_rcItem);
	PaintStatusImage(hDC);
	DrawItemText(hDC, m_rcItem);
}


void CMyListUI::PaintStatusImage(HDC hDC)
{
	if (!mIsLocked)
	{
		CRect rcPos;
		CDuiString dest;
		rcPos = { 825,23,851,55 };
		dest.Format(_T("file='Button\\��2.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}

	if(prereadMessageNum!=0)
	{
		CRect rcPos;
		CDuiString dest;
		rcPos = {549,11,584,46};
		dest.Format(_T("file='Button\\��Բ��.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}
}

void CMyListUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
	CVideoListUI::Node* node = (CVideoListUI::Node*)this->GetTag();

	CDuiString sText = node->data()._text;
	if (sText.IsEmpty()) return;

	if (m_pOwner == NULL) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	DWORD iTextColor = pInfo->dwTextColor;
	if ((m_uButtonState & UISTATE_HOT) != 0) {
		iTextColor = pInfo->dwHotTextColor;
	}
	if (IsSelected()) {
		iTextColor = 0xFFFFFFFF;
	}
	if (!IsEnabled()) {
		iTextColor = pInfo->dwDisabledTextColor;
	}
	int nLinks = 0;
	RECT rcText = rcItem;
	if(node->data()._level==0)//ͷ�������
		pInfo->nFont = 2;
	else pInfo->nFont = 3;	//�ӽڵ�

	pInfo->uTextStyle &= ~(DT_LEFT | DT_RIGHT);
	pInfo->uTextStyle |= DT_CENTER;
	CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, iTextColor, \
		pInfo->nFont, pInfo->uTextStyle);
}

void CMyListUI::DoEvent(TEventUI & event)
{
	if (event.Type == UIEVENT_KEYDOWN){
		if (event.wParam == VK_RETURN){
			CVideoListUI *pList = (CVideoListUI*)GetParent()->GetParent();
			int num;
			CVideoListUI::Node* node = (CVideoListUI::Node*)GetTag();
			if (node->has_children()){
				static_cast<CVideoListUI*>(pList)->ExpandNode(node, !node->data()._expand);
			}
			else
			{

			}
			if (node->data()._level == 0 && node->data()._expand) {
				pList->SelectItem(pList->GetItemIndex(this) + 1);
				pList->GetItemAt(pList->GetItemIndex(this) + 1)->SetFocus();
			}
		}
	/*	else{
			CListLabelElementUI::DoEvent(event);
		}*/
	}
	//else{
	//	CListLabelElementUI::DoEvent(event);
	//}
	CListLabelElementUI::DoEvent(event);
}


