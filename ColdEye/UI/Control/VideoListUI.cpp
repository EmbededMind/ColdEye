#include "stdafx.h"
#include "VideoListUI.h"

IMPLEMENT_DUICONTROL(CVideoListUI)
void CVideoListUI::DoEvent(TEventUI& event)
{
	switch (event.Type) {
	case UIEVENT_KEYDOWN:
		switch (event.chKey) {
		case VK_UP:
			{
				CListLabelElementUI *pItem = (CListLabelElementUI*)m_pList->GetItemAt(GetMaxSelItemIndex());
				CVideoListUI::Node* node = (CVideoListUI::Node*)pItem->GetTag();
				if (node->data()._level != 0)
				{
					if (m_aSelItems.GetSize() > 0) {
						int index = GetMinSelItemIndex() - 1;
						UnSelectAllItems();
						index > 0 ? SelectItem(index, true) : SelectItem(0, true);
					}
				}
				else
				{
					if (GetMaxSelItemIndex() != 0) //����
					{
						CListLabelElementUI *pItem = (CListLabelElementUI*)m_pList->GetItemAt(GetMaxSelItemIndex()-1);
						CVideoListUI::Node* node = (CVideoListUI::Node*)pItem->GetTag();
						CVideoListUI::Node* parentnode = node->parent();
						SelectItem(GetMaxSelItemIndex() - 1 - parentnode->num_children(), true);
					}
				}
			}
			break;
		case VK_DOWN:
			{
				CListLabelElementUI *pItem = (CListLabelElementUI*)m_pList->GetItemAt(GetMaxSelItemIndex());
				CVideoListUI::Node* node = (CVideoListUI::Node*)pItem->GetTag();
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
					if(GetMaxSelItemIndex()+1+node->num_children()<m_pList->GetCount()) //�����ж�
						SelectItem(GetMaxSelItemIndex()+1 + node->num_children(), true);
				}
			}
			break;
		}
		break;

	case UIEVENT_BUTTONDOWN:
		CListUI:DoEvent(event);
		break;
	}
}