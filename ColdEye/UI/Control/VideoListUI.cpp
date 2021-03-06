#include "stdafx.h"
#include "VideoListUI.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CVideoListUI)
void CVideoListUI::DoEvent(TEventUI& event)
{
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		{
			CListLabelElementUI *pItem = (CListLabelElementUI*)GetItemAt(GetMaxSelItemIndex());	
			if (!pItem)return;
			CVideoListUI::Node* node = (CVideoListUI::Node*)pItem->GetTag();
			switch (event.wParam) {
				case VK_UP:
				{
					//头结点移动和子节点移动
					if (node->data()._level != 0)
					{
						pItem = (CListLabelElementUI*)GetItemAt(GetMaxSelItemIndex() - 1);
						node = (CVideoListUI::Node*)pItem->GetTag();
						if (node->data()._level == 0){

						}
						else if (m_aSelItems.GetSize() > 0) {
							int index = GetMinSelItemIndex() - 1;
							UnSelectAllItems();
							index > 0 ? SelectItem(index, true) : SelectItem(0, true);
						}
					}
					else
					{
						
						if (GetMaxSelItemIndex() != 0){
							pItem = (CListLabelElementUI*)m_pList->GetItemAt(GetMaxSelItemIndex() - 1);
							node = (CVideoListUI::Node*)pItem->GetTag();
							CVideoListUI::Node* parentnode = node->parent();
							SelectItem(GetMaxSelItemIndex() - 1 - parentnode->num_children(), true);
						}
					}
				}
				break;
				//------------------------------------------------------------
				case VK_DOWN:
				{
					if (node->data()._level != 0) { //子节点移动焦点
						CVideoListUI::Node* parentnode = node->parent();
						int num_node = parentnode->num_children();
						if (m_aSelItems.GetSize() > 0) {
							int index = GetMaxSelItemIndex() + 1;
							UnSelectAllItems();
							if (parentnode->child(0)->data()._pListElement == node->data()._pListElement)
							{
								SelectItem(index - 1, true);
							}
							else
							{
								SelectItem(index, true);
							}
						}
					}
					else //头结点移动焦点
					{
						CVideoListUI::Node* parentnode = node->parent();
						if (GetMaxSelItemIndex() + 1 + node->num_children() < m_pList->GetCount()) //下限判断
							SelectItem(GetMaxSelItemIndex() + 1 + node->num_children(), true);
					}
				}
				break;
			}
		}
		break;
	}

}

CVideoListUI::Node * CVideoListUI::AddHeadNode(CString sText, int nID, CRecordFileInfo* pInfo)
{
	CVideoListUI::Node* pNode = AddNode(sText, nID,pInfo);
	pNode->data()._pListElement->SetBkColor(0XFFF3F3F3);
	pNode->data()._pListElement->SetFixedHeight(m_listElementHight);
	return pNode;
}

CVideoListUI::Node * CVideoListUI::AddChildNode(CString sText, CVideoListUI::Node * HeadNode, int nID,CRecordFileInfo* pInfo)
{
	CVideoListUI::Node* pNode = AddNode(sText, nID,pInfo, HeadNode);
	if (HeadNode->num_children() % 2)
	{
		pNode->data()._pListElement->SetBkColor(0xFFF3F3F3);
	}
	else
	{
		pNode->data()._pListElement->SetBkColor(0xFFE5E5E5);
	}
	pNode->data()._pListElement->SetFixedHeight(m_listElementHight);
	return pNode;
}

void CVideoListUI::AddRecordFile(CRecordFileInfo* pInfo)
{
	AddItem(pInfo);
	RefreshList();

	//报警视频列表 中计算未看视频记录
	CDuiString name = GetName();
	name.Assign(name, name.GetLength() - 1);
	if (_tcscmp(name, _T("video_alarmlist")) == 0) {
		if (pInfo->status == RECORD_NSEEN) {
			CMyListUI *pItem = (CMyListUI*)GetItemAt(0);
			pItem->mhintNumber++;
		}
	}

}

void CVideoListUI::AddItem(CRecordFileInfo * pInfo)
{
	if (!pInfo) return;
	CTime refTime = CTime::GetCurrentTime();
	CMyListUI *pItem = (CMyListUI*)GetItemAt(0);
	CTime tbegin = CTime(pInfo->tBegin);
	CTime tend = CTime(pInfo->tEnd);
	CVideoListUI::Node* pNode = NULL;
	if (pItem) {
		pNode = (CVideoListUI::Node*)pItem->GetTag();
	}


	//判断是否存在节点
	if (pItem) {
		if (IsNewDay(pItem->Info, pInfo)) {
			CompareBeginTime(refTime, tbegin, tend, pInfo, pNode);
		}
		else {
			pNode = AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
		}
	}
	else { //无任何头结点	
		CompareBeginTime(refTime, tbegin, tend, pInfo, pNode);
	}

}

void CVideoListUI::RefreshList()
{
	int num;
	CTime tbegin;
	int SpanDay;
	CTime refTime = CTime::GetCurrentTime();
	CVideoListUI::Node* pNode  = NULL;
	CMyListUI *pItem = (CMyListUI*)GetItemAt(0);
	if (!pItem) return;
	//第一头结点
	pNode = (CVideoListUI::Node*)pItem->GetTag();
	SpanDay = DataSpan(refTime, CTime(pItem->Info->tBegin));
	if (SpanDay == 0) {
		pNode->data()._text = _T("今天");
	}
	else if (SpanDay == 1) {
		pNode->data()._text = _T("昨天");
	}
	else {
		tbegin = CTime(pItem->Info->tBegin);
		pNode->data()._text = tbegin.Format("%Y-%m-%d");
	}

	num = pNode->num_children();
	if (num) {
		pItem = (CMyListUI*)GetItemAt(num + 1); //第二个头节点
		if (!pItem) return;
		pNode = (CVideoListUI::Node*)pItem->GetTag();
		SpanDay = DataSpan(refTime, CTime(pItem->Info->tBegin));
		if (SpanDay == 0) {
			pNode->data()._text = _T("今天");
		}
		else if (SpanDay == 1) {
			pNode->data()._text = _T("昨天");
		}
		else {
			tbegin = CTime(pItem->Info->tBegin);
			pNode->data()._text = tbegin.Format("%Y-%m-%d");
		}

		num += pNode->num_children(); //第三个头结点
		if (!num) {
			pItem = (CMyListUI*)GetItemAt(num);
			if (!pItem) return;
			pNode = (CVideoListUI::Node*)pItem->GetTag();
			if (!pItem) return;

			tbegin = CTime(pItem->Info->tBegin);
			pNode->data()._text = tbegin.Format("%Y-%m-%d");
		}
	}
}


void CVideoListUI::DeleteRecordFile(CRecordFileInfo* pInfo)
{
	int Count=GetCount();
	CMyListUI *pListItem;
  	Node *node;
	for (int i = 0; i < Count; i++) {
		pListItem = (CMyListUI*)GetItemAt(i);
		node = (Node*)pListItem->GetTag();
		if (node->data()._pListElement->Info->tBegin == pInfo->tBegin) {
			if (node->data()._level==0) {  //删除的结点如果在当天记录中最后一个则将头结点也删除
				if (node->num_children() == 1) {
					RemoveNode(node);
					break;
				}
			}
			else
				RemoveNode(node);
				break;
		}
		
	}
	
}

void CVideoListUI::CompareBeginTime(CTime refTime, CTime tbegin, CTime tend, CRecordFileInfo * pInfo, CVideoListUI::Node* pNode)
{
	if (refTime.GetYear() == tbegin.GetYear() && refTime.GetMonth() == tbegin.GetMonth() && refTime.GetDay() == tbegin.GetDay()) {
		pNode = AddHeadNode(_T("今天"), 0, pInfo);
		AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
	}
	else if (refTime.GetYear() == tbegin.GetYear() && refTime.GetMonth() == tbegin.GetMonth() && refTime.GetDay() - tbegin.GetDay() == 1) {
		pNode = AddHeadNode(_T("昨天"), 0, pInfo);
		AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);	
	}	
	else {
			pNode = AddHeadNode(tbegin.Format("%Y-%m-%d"), 0, pInfo);
			AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
		}
}

int CVideoListUI::IsNewDay(CRecordFileInfo *HeadInfo,CRecordFileInfo * pInfo)
{
	int year, month, day, InfoYear,InfoMonth,InfoDay;
	//头结点年月日
	year = CTime(HeadInfo->tBegin).GetYear();
	month = CTime(HeadInfo->tBegin).GetMonth();
	day = CTime(HeadInfo->tBegin).GetDay();

	InfoYear = CTime(pInfo->tBegin).GetYear();
	InfoMonth = CTime(pInfo->tBegin).GetMonth();
	InfoDay = CTime(pInfo->tBegin).GetDay();
	if (year != InfoYear)
		return 1;
	if (month != InfoMonth)
		return 1;
	if (day != InfoDay)
		return 1;

	return 0;
}

void CVideoListUI::SetListElementHeight(float m_scale)
{
	m_listElementHight = 60 * m_scale;
}


//时间差 
int CVideoListUI::DataSpan(CTime refTime, CTime _tBegin)
{
	if (refTime.GetYear() == _tBegin.GetYear() && refTime.GetMonth() == _tBegin.GetMonth() && refTime.GetDay() == _tBegin.GetDay()) {
		return 0;
	}
	else if (refTime.GetYear() == _tBegin.GetYear() && refTime.GetMonth() == _tBegin.GetMonth()) {
		if (refTime.GetDay() - _tBegin.GetDay() == 1) {
			return 1;
		}
		else {
			return 2;
		}
	}
	//跨月
	else if (refTime.GetYear() == _tBegin.GetYear()) {
		if (refTime.GetMonth() - _tBegin.GetMonth() == 1) {

			CTime refTimeTemp = CTime(refTime.GetYear(), refTime.GetMonth(), refTime.GetDay(), 0, 0, 0);
			CTimeSpan span = refTimeTemp.GetTime() - _tBegin.GetTime();
			if (span.GetDays() == 0) {
				return 1;
			}
			else {
				return 2;
			}
		}
		else {
			return 2;
		}
	}
	//跨年
	else {
		CTime refTimeTemp = CTime(refTime.GetYear(), refTime.GetMonth(), refTime.GetDay(), 0, 0, 0);
		CTimeSpan span = refTimeTemp.GetTime() - _tBegin.GetTime();
		if (span.GetDays() == 0) {
			return 1;
		}
		else {
			return 2;
		}
	}
}


