#include "stdafx.h"
#include "MyListUI.h"
#include "VideoListUI.h"
#include "Wnd\MsgWnd.h"
#include <list>

IMPLEMENT_DUICONTROL(CMyListUI)
CMyListUI::CMyListUI()
	:mhintNumber(0),
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
	if (Info->status== RECORD_LOCKED)
	{
		CRect rcPos;
		CDuiString dest;
		rcPos = { 825,23,851,55 };
		dest.Format(_T("file='image\\��2.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
	}
	if(mhintNumber !=0)
	{
		CDuiString text;
		if(mhintNumber<99)
			text.Format(_T("%d"), mhintNumber);
		else 
			text=_T("99");
		RECT rcPos = { 549,11,584,46 };
		RECT textPos = GetPos();
		CDuiString dest;

		if (mhintNumber>9) {
			textPos.left += (rcPos.left + 4);
			textPos.top += (rcPos.top + 7);
			textPos.right = textPos.left + 50;
			textPos.bottom = textPos.top + 50;
		}
		else {
			textPos.left += (rcPos.left+11);
			textPos.top += (rcPos.top+7);
			textPos.right = textPos.left +50;
			textPos.bottom = textPos.top + 50;
		}

		dest.Format(_T("file='image\\��Բ��.png' dest='%d,%d,%d,%d'"), rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
		DrawImage(hDC, dest);
		CRenderEngine::DrawText(hDC, m_pManager, textPos, text, 0xFFFFFF, 1, 0);
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
				CDuiString name = GetParent()->GetParent()->GetName();
				CMyListUI* pElement;
				int inx;
				inx = pList->GetItemIndex(this);
				while (pList->GetItemAt(inx)) {
					pElement = static_cast<CMyListUI*>(pList->GetItemAt(inx));
					node = (CVideoListUI::Node*)pElement->GetTag();
					if(node->data()._level==1)
						infoForPlayer.push_back(pElement->Info);
					inx++;
				}
				name.Assign(name,name.GetLength() - 1);
				if (_tcscmp(name, _T("video_alarmlist")) == 0) {
					event.wParam = RECORD_ALARM;
				}
				else {
					event.wParam = RECORD_NORMAl;
				}
				m_pManager->SendNotify(this, DUI_MSGTYPE_PLAYER, event.wParam, (LPARAM)&infoForPlayer);
				//Print("count:%d", listInfo.size());
			}
			if (node->data()._level == 0 && node->data()._expand) {
				pList->SelectItem(pList->GetItemIndex(this) + 1);
				pList->GetItemAt(pList->GetItemIndex(this) + 1)->SetFocus();
			}
		}

		if (event.wParam == VK_BACK) {
			CVideoListUI *pList = (CVideoListUI*)GetParent()->GetParent();
			CVideoListUI::Node* node = (CVideoListUI::Node*)GetTag();
			if (node->data()._level == 1) {
				pList->ExpandNode(node->parent(), 0);
				node->parent()->data()._pListElement->SetFocus();
				node->parent()->data()._pListElement->Select(true);
			}
			else {
				m_pManager->SendNotify(this, DUI_MSGTYPE_LISTLABEL, event.wParam, event.lParam);
			}
		}

		if (GetKeyState(VK_CONTROL) && !(event.wParam & 0x20000000)) {
			if (event.wParam == 'U') { //U�̼�
				list<CRecordFileInfo*> RecordInfo;
				UINT8 type;
				RecordInfo = FindRecordFile(&type);
				m_pManager->SendNotify(this, DUI_MSGTYPE_COPYFILE, type, (LPARAM)&RecordInfo);
			}
		}

		if (GetKeyState(VK_CONTROL) && !(event.wParam & 0x20000000)) {
			if (event.wParam == 'L') { //����
				CVideoListUI::Node* node = (CVideoListUI::Node*)GetTag();
				if (node->data()._level == 1) {
					m_pManager->SendNotify(this, DUI_MSGTYPE_LISTLABEL, event.wParam, event.lParam);
				}
			}
		}
	}
	CListLabelElementUI::DoEvent(event);
}

list<CRecordFileInfo*> CMyListUI::FindRecordFile(UINT8* type)
{
	UINT8 RecordType;
	int num_record,TabLayoutSel;
	CVideoListUI::Node* Headnode = (CVideoListUI::Node*)GetTag();
	CVideoListUI::Node* pNode;
	list<CRecordFileInfo*> RecordInfoList;
	num_record = Headnode->num_children();
	TabLayoutSel = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->GetCurSel();
	//�ж���Ƶ�ļ�����
	if (TabLayoutSel > 0 && TabLayoutSel < 6)
		*type = 1;

	else
		*type = 0;
	if (num_record){//����
		for (int i = 0; i < num_record; i++) {
			pNode = Headnode->child(i);
			RecordInfoList.push_back(pNode->data()._pListElement->Info);
		}
	}
	else {
		RecordInfoList.push_back(Headnode->data()._pListElement->Info);
	}
	return RecordInfoList;
	//SendMessage(m_pManager->GetPaintWindow(), USER_MSG_MESSAGE_BOX, RecordType, (LPARAM)(&RecordInfoList));
}


