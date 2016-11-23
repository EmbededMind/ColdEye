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
			CVideoListUI::Node* node = (CVideoListUI::Node*)pItem->GetTag();
			switch (event.wParam) {
				case VK_UP:
				{
					//ͷ����ƶ����ӽڵ��ƶ�
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
					if (node->data()._level != 0) { //�ӽڵ��ƶ�����
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
					else //ͷ����ƶ�����
					{
						CVideoListUI::Node* parentnode = node->parent();
						if (GetMaxSelItemIndex() + 1 + node->num_children() < m_pList->GetCount()) //�����ж�
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
	return pNode;
}

void CVideoListUI::AddRecordFile(CRecordFileInfo* pInfo)
{
	CTime refTime = CTime::GetCurrentTime();
	CMyListUI *pItem = (CMyListUI*)GetItemAt(0);
	CTime tbegin = CTime(pInfo->tBegin);
	CTime tend = CTime(pInfo->tEnd);
	CVideoListUI::Node* pNode;
	//List �����ļ��Ѵ����ļ�
	if (pItem){//���һ���ڵ�����ͬһ�죬��ӵ���ǰ�ڵ���
		if (pItem->Info->tBegin / (3600 * 24) == refTime.GetTime() / (3600 * 24)){
			pNode = (CVideoListUI::Node*)pItem->GetTag();
			AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"),pNode, 0, pInfo);
		}
		else {//��ͬһ�죬����һ���ڵ㣬����һ���ڵ�������죬�ڶ����ڵ���ʾ����Ϊ�ýڵ��ʱ��
			pNode = (CVideoListUI::Node*)pItem->GetTag();
			pNode->data()._text = _T("����");
			pNode = AddHeadNode(_T("����"),0,pInfo);
			AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
			pItem = (CMyListUI*)GetItemAt(pNode->num_children() + 3);
			if (pItem)
			{
				tbegin = CTime(pItem->Info->tBegin);
				pNode = (CVideoListUI::Node*)pItem->GetTag();
				pNode->data()._text = tbegin.Format("%Y-%m-%d");
			}
		}
	}
	else //List��δ����κ��ļ�
	{
		pNode = AddHeadNode(_T("����"), 0, pInfo);
		tbegin = CTime(pInfo->tBegin);
		tend = CTime(pInfo->tEnd);
		AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"), pNode, 0, pInfo);
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
			if (node->data()._level==0) {  //ɾ���Ľ������ڵ����¼�����һ����ͷ���Ҳɾ��
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

