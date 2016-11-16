#include "stdafx.h"
#include "VideoListUI.h"
#include "conio.h"

IMPLEMENT_DUICONTROL(CVideoListUI)
void CVideoListUI::DoEvent(TEventUI& event)
{
	_cprintf("type=%d\n",event.Type);
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		{
			CListLabelElementUI *pItem = (CListLabelElementUI*)GetItemAt(GetMaxSelItemIndex());
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

CVideoListUI::Node * CVideoListUI::AddHeadNode(CString sText, int nID, CRecordFileInfo& pInfo)
{
	CVideoListUI::Node* pNode = AddNode(sText, nID,pInfo);
	pNode->data()._pListElement->SetBkColor(0XFFF3F3F3);
	return pNode;
}

CVideoListUI::Node * CVideoListUI::AddChildNode(CString sText, CVideoListUI::Node * HeadNode, int nID,CRecordFileInfo& pInfo)
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
	return pNode;
}

void CVideoListUI::Record_AddFile(CRecordFileInfo & pInfo)
{
	CTime refTime = CTime::GetCurrentTime();
	CMyListUI *pItem = (CMyListUI*)GetItemAt(0);
	CTime tbegin = CTime(pInfo.tBegin);
	CTime tend = CTime(pInfo.tEnd);
	CVideoListUI::Node* pNode;
	//如果第一次添加，先添加头结点，再添加子节点
	if (pItem){
		if (pItem->mBeginTime / (3600 * 24) == refTime.GetTime() / (3600 * 24)){//同一天记录
			pNode = (CVideoListUI::Node*)pItem->GetTag();
			AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"),pNode, 0, pInfo);
		}
		else {
			pNode = (CVideoListUI::Node*)pItem->GetTag();
			pNode->data()._text = _T("昨天");
			pNode = AddHeadNode(_T("今天"),0,pInfo);
			AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
			pItem = (CMyListUI*)GetItemAt(pNode->num_children() + 3);
			if (pItem)
			{
				tbegin = CTime(pItem->mBeginTime);
				pNode = (CVideoListUI::Node*)pItem->GetTag();
				pNode->data()._text = tbegin.Format("%Y-%m-%d");
			}
		}
	}
	else //第一次添加
	{
		pNode = AddHeadNode(_T("今天"), 0, pInfo);
		tbegin = CTime(pInfo.tBegin);
		tend = CTime(pInfo.tEnd);
		AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
	}
}

