#include "stdafx.h"
#include "MyListUI.h"
#include "VideoListUI.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CMyListUI)
CMyListUI::CMyListUI()
	:mIsLocked(false),
	prereadMessageNum(0)
{
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
	_cprintf("level=%d\n",node->data()._level);

	CDuiString sText = GetText();
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

