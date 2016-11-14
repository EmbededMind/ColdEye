#include "stdafx.h"
//
#include "Wnd\MyMenuWnd.h"
#include "Control\PopupMenuUI.h"
#include "Control\MenuItemUI.h"
#include "Control\TimeButtonUI.h"
#include "Control\VideoListUI.h"


CMyMenuWnd::CMyMenuWnd()
{
}

CMyMenuWnd::~CMyMenuWnd()
{
}


LPCTSTR CMyMenuWnd::GetWindowClassName() const
{
	return _T("MyMenuWnd");
}


CDuiString CMyMenuWnd::GetSkinFile()
{
	return _T("ui.xml");
}

void CMyMenuWnd::InitWindow()
{
	//-------------------------�˵��ؼ�����-----------------------------------------
	CPopupMenuUI *pPopupMenu[5];
	pPopupMenu[0] = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("alarmvideo")));
	pPopupMenu[1] = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("setting")));
	pPopupMenu[2] = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("autowatch")));
	pPopupMenu[3] = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("videoget")));
	pPopupMenu[4] = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("homewatch")));
	pPopupMenu[0]->SetItemRelation(NULL, pPopupMenu[1]);
	pPopupMenu[1]->SetItemRelation(pPopupMenu[0], pPopupMenu[2]);
	pPopupMenu[2]->SetItemRelation(pPopupMenu[1], pPopupMenu[3]);
	pPopupMenu[3]->SetItemRelation(pPopupMenu[2], pPopupMenu[4]);
	pPopupMenu[4]->SetItemRelation(pPopupMenu[3], NULL);
	
	CMenuItemUI *pMenuItem,*pPrevItem, *pNextItem;
	pMenuItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("mastername")));
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("sysset")));
	pMenuItem->SetItemRelation(NULL, pNextItem);
	pNextItem->SetItemRelation(pMenuItem, NULL);

	pMenuItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("watchtime")));
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("alarm_voice")));
	pMenuItem->SetItemRelation(NULL, pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("alarm_light")));
	pMenuItem->SetItemRelation(pPrevItem,pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("watchrecode")));
	pMenuItem->SetItemRelation(pPrevItem, pNextItem);
	pNextItem->SetItemRelation(pMenuItem, NULL);

	pMenuItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera1_video")));
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera2_video")));
	pMenuItem->SetItemRelation(NULL, pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera3_video")));
	pMenuItem->SetItemRelation(pPrevItem, pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera4_video")));
	pMenuItem->SetItemRelation(pPrevItem, pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera5_video")));
	pMenuItem->SetItemRelation(pPrevItem, pNextItem);
	pPrevItem = pMenuItem;
	pMenuItem = pNextItem;
	pNextItem = static_cast<CMenuItemUI*>(m_pm.FindControl(_T("camera6_video")));
	pMenuItem->SetItemRelation(pPrevItem, pNextItem);
	pNextItem->SetItemRelation(pMenuItem, NULL);

	//-------------------------����ʱ��ؼ�����---------------------------------
	CTimeButtonUI *pTimeBt[4]; 
	pTimeBt[0] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_hour")));
	pTimeBt[1] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_minute")));
	pTimeBt[2] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_hour")));
	pTimeBt[3] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_minute")));
	pTimeBt[0]->SetItemRelation(NULL, pTimeBt[1]);
	pTimeBt[1]->SetItemRelation(pTimeBt[0], pTimeBt[2]);
	pTimeBt[2]->SetItemRelation(pTimeBt[1], pTimeBt[3]);
	pTimeBt[3]->SetItemRelation(pTimeBt[2], NULL);

	//-------------------------��������ý������-------------------------------
	CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	CDialogBuilder Camera1builder, Camera2builder, Camera3builder, Camera4builder, Camera5builder, Camera6builder;
	CVerticalLayoutUI *CamareChildLayout;
	CamareChildLayout = (CVerticalLayoutUI*)Camera1builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 7);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ1����"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera2builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 8);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ2����"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera3builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 9);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ3����"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera4builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 10);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ4����"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera5builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 11);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ5����"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera6builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 12);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("����ͷ6����"));

	//�������ͷ
	AddCamear();

	CRecordFileInfo *pRecord = new CRecordFileInfo;
	pRecord->nOwner = 1;
	pRecord->tBegin = CTime::GetCurrentTime().GetTime();
	pRecord->tEnd = CTime::GetCurrentTime().GetTime();
	//Record_AddFile(1, *pRecord);
}

void CMyMenuWnd::OnFinalMessage(HWND hWnd)
{
}

void CMyMenuWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == _T("itemactivate")) {
		CVideoListUI* pList = static_cast<CVideoListUI*>(m_pm.FindControl(_T("video_alarmlist1")));
		if (pList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0) {
				CVideoListUI::Node* node = (CVideoListUI::Node*)msg.pSender->GetTag();
				pList->ExpandNode(node, !node->data()._expand);

				if (node->data()._level == 0 && node->data()._expand) {
					pList->SelectItem(pList->GetItemIndex(msg.pSender) + 1);
				}
			}
		}
	}
}

void CMyMenuWnd::OnLClick(CControlUI * pControl)
{
}

LRESULT CMyMenuWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT CMyMenuWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	//if (uMsg == UIMSG_ADDNODE)
	//{
	//	CDuiString ListName;
	//	ListName.Format(_T("list%d", wParam));
	//	CVideoListUI* pList = static_cast<CVideoListUI*>(m_pm.FindControl(ListName));
	//}

	return LRESULT();
}

void CMyMenuWnd::AddCamear()
{ 
	CVerticalLayoutUI *pLayout;
	CMenuItemUI *pMenuItem;
	CDuiString text, userdata, name;
	int Count;
	//������Ƶ�����˵�
	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("layout_submenu_alarm")));
	Count = pLayout->GetCount()/2+1;
	text.Format(_T("����ͷ%d"), Count);
	pMenuItem = new CMenuItemUI(pLayout, text);
	userdata.Format(_T("%d"), Count - 1);
	pMenuItem->SetUserData(userdata);
	if (Count == 1)
	{
		pMenuItem->SetPrevItem(NULL);
		pMenuItem->SetNextItem(NULL);
	}
	else
	{
		static_cast<CMenuItemUI*>(pLayout->GetItemAt(Count - 2))->SetNextItem((CControlUI*)pMenuItem);
		pMenuItem->SetPrevItem((CControlUI*)pLayout->GetItemAt(Count - 2));
		pMenuItem->SetNextItem(NULL);
	}
	
	//����ͷ���ÿؼ�
	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("layout_submenu_setting")));
	text.Format(_T("����ͷ%d"), Count);
	pMenuItem = new CMenuItemUI(pLayout, text, (Count * 2));
	userdata.Format(_T("%d"), Count + 6);	//���ÿؼ�����
	name.Format(_T("cameraset%d"), Count);
	pMenuItem->SetName(name);					//���ÿؼ�����
	pMenuItem->SetUserData(userdata);
	static_cast<CMenuItemUI*>(pLayout->GetItemAt((Count - 1) * 2))->SetNextItem((CControlUI*)pMenuItem); //��������
	pMenuItem->SetPrevItem((CControlUI*)(pLayout->GetItemAt((Count - 1) * 2))); 
	pMenuItem->SetNextItem((CControlUI*)pLayout->GetItemAt((Count+1)*2)); 
	static_cast<CMenuItemUI*>(pLayout->GetItemAt((Count + 1) * 2))->SetPrevItem((CControlUI*)pMenuItem); //ϵͳ����
}


//void CMyMenuWnd::Record_AddFile(UINT8 record_type, CRecordFileInfo& pInfo)
//{
//	CTime refTime = GetCurrentTime();
//	CTime tbegin, tend;
//	CDuiString name;
//	CVideoListUI *pList;
//	CMyListUI *pItem;
//	//ȫ����Ƶ
//	if (record_type == 1) {
//		name.Format(_T("video_list%d"), pInfo.nOwner);
//		CVideoListUI *pList = (CVideoListUI*)m_pm.FindControl(name);
//		pItem = (CMyListUI*)pList->GetItemAt(0);
//		if (pItem) {
//			if (pItem->mBeginTime.GetTime() / (3600 * 24) == refTime.GetTime() / (3600 * 24)) {
//			}
//			else {
//				pList->AddHeadNode(_T("����"), 0, pList);
//			}
//		}
//		else {
//			pList->AddHeadNode(_T("����"), 0, pList);
//
//		}
//	}
//	else {//������Ƶ
//		name.Format(_T("video_alarmlist%d"), pInfo.nOwner);
//		pList = (CVideoListUI*)m_pm.FindControl(name);
//		pItem = (CMyListUI*)pList->GetItemAt(0);
//		if (pItem) {
//			if (pItem->mBeginTime.GetTime() / (3600 * 24) == refTime.GetTime() / (3600 * 24)) {
//			}
//			else {
//				pList->AddHeadNode(_T("����"), 0, pList);
//			}
//		}
//		else { //��һ�����
//			CVideoListUI::Node* pNode = pList->AddHeadNode(_T("����"), 0, pList);
//			tbegin = CTime(pInfo.tBegin);
//			tend = CTime(pInfo.tEnd);
//			pList->AddChildNode(tbegin.Format("%Y-%m-%d  %H:%M") + _T("-") + tend.Format("%H:%M"),pList, pNode, 0);
//		}
//	}
//}