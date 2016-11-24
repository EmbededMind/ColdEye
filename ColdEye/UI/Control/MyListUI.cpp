#include "stdafx.h"
#include "MyListUI.h"
#include "VideoListUI.h"
#include "Wnd\MsgWnd.h"
#include <list>

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
		dest.Format(_T("file='Button\\锁2.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}

	if(prereadMessageNum!=0)
	{
		CRect rcPos;
		CDuiString dest;
		rcPos = {549,11,584,46};
		dest.Format(_T("file='Button\\椭圆形.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
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
	if(node->data()._level==0)//头结点字体
		pInfo->nFont = 2;
	else pInfo->nFont = 3;	//子节点

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

		if (GetKeyState(VK_CONTROL) && !(event.wParam & 0x20000000)) {
			if (event.wParam == 'U') { //U盘键
				CVideoListUI::Node* node = (CVideoListUI::Node*)GetTag();
				CTime tbegin, tend;
				CDuiString text;
				if (node->data()._level == 0) {
					text = GetText();
				}
				else {
					tbegin = CTime(Info->tBegin);
					tend = CTime(Info->tEnd);
					text = tbegin.Format("%m月%d日   ") + tbegin.Format("%H:%M:%S") + _T("-") + tend.Format("%H:%M:%S");
				}
				CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_copyvideo_request.xml"), text, NULL, NULL,NULL);
				FindRecordFile();
			}
		}

	}
	CListLabelElementUI::DoEvent(event);
}

void CMyListUI::FindRecordFile()
{
	UINT8 RecordType;
	int num_record,TabLayoutSel;
	CVideoListUI::Node* Headnode = (CVideoListUI::Node*)GetTag();
	CVideoListUI::Node* pNode;
	list<CRecordFileInfo*> RecordInfoList;
	num_record = Headnode->num_children();
	TabLayoutSel = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->GetCurSel();
	//判断视频文件类型
	if (TabLayoutSel > 0 && TabLayoutSel < 6)
		RecordType = COPYING_ALARM;
	else
		RecordType = COPYING_NORMAL;
	if (num_record){//整天
		for (int i = 0; i < num_record; i++) {
			pNode = Headnode->child(i);
			RecordInfoList.push_back(pNode->data()._pListElement->Info);
		}
	}
	else {
		RecordInfoList.push_back(Headnode->data()._pListElement->Info);
	}
	
	SendMessage(m_pManager->GetPaintWindow(), USER_MSG_MESSAGE_BOX, RecordType, (LPARAM)(&RecordInfoList));
}


