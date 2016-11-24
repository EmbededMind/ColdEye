#include "stdafx.h"

#include "Control\MenuItemUI.h"
#include "Control\MyEditUI.h"
#include "ExHardDrive\ExHardDrive.h"
#include "File\RecordFileInfo.h"
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

IMPLEMENT_DUICONTROL(CMenuItemUI)

CMenuItemUI::CMenuItemUI()
{}

CMenuItemUI::CMenuItemUI(CContainerUI* pParent, CString& text)
{
	mHintNumber = _T("0");
	SetText(text);

	pParent->Add(this);
	SetAttribute(_T("style"), _T("MenuItem"));

	CControlUI * p = new CControlUI;
	pParent->Add(p);
	p->SetAttribute(_T("style"), _T("control"));
}


CMenuItemUI::CMenuItemUI(CContainerUI* pParent, CString& text, CDuiString userdata, int iInx)
{
	mHintNumber = _T("0");

	SetText(text);
	pParent->AddAt(this, iInx);
	SetAttribute(_T("style"), _T("MenuItem"));
	SetUserData(userdata);

	CControlUI* p = new CControlUI;
	pParent->AddAt(p, iInx+1);
	p->SetAttribute(_T("style"), _T("control"));
}

CMenuItemUI::~CMenuItemUI()
{}


void CMenuItemUI::SetItemBkColor(CControlUI* pfocusItem,DWORD Color1, DWORD Color2)
{
	CContainerUI *pItem;
	//焦点移动到一级菜单上
	if (pfocusItem) {
		CContainerUI *layout = (CContainerUI*)pfocusItem->GetParent();
		layout->SetBkColor(Color2);
		for (int i = 0; i < layout->GetCount(); i += 2) {
			if (layout->GetItemAt(i) != pfocusItem)
				layout->GetItemAt(i)->SetBkColor(Color2);
		}
		static_cast<CButtonUI*>(pfocusItem)->SetTextColor(0xFF666666);

		layout = (CContainerUI*)GetParent();
		layout->SetBkColor(Color1);
		for (int i = 0; i < layout->GetCount(); i += 2) {
			layout->GetItemAt(i)->SetBkColor(Color1);
		}
	}//焦点移动下一级
	else{
		CContainerUI *layout = (CContainerUI*)GetParent();
		layout->SetBkColor(Color1);
		for (int i = 0; i < layout->GetCount(); i += 2) {
			pItem = (CContainerUI*)layout->GetItemAt(i);
			if(pItem!=(CContainerUI*)this)
				pItem->SetBkColor(Color1);
			else {
				SetBkColor(0xFF4178B7);
				SetTextColor(0xFFFFFFFF);
			}
		}

	}

}



void CMenuItemUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_KEYDOWN) {
		if (GetKeyState(VK_CONTROL)) {
			if (event.wParam=='U') {
				FindRecordFile();
				return;
			}
		}

		switch (event.wParam)
		{
			
			//----------------------------------------------------
			case VK_UP:
				if (mPrevItem) {
					mPrevItem->SetFocus();
				}
				break;
		    //----------------------------------------------------
			case VK_DOWN:
				if (mNextItem) {
					mNextItem->SetFocus();
				}
				break;
 			//----------------------------------------------------
			case VK_LEFT:
				{
					CControlUI* pFocusedItem = NULL;
					CContainerUI *layout;
					static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SetVisible(false);
					int userData = StrToInt(GetUserData());

					if (userData < 6) {
						pFocusedItem = m_pManager->FindControl(_T("alarmvideo"));
					}
					else if (userData >= 6  &&  userData < 14) {
						pFocusedItem = m_pManager->FindControl(_T("setting"));
					}
					else if (userData >= 14 && userData < 18) {
						pFocusedItem = m_pManager->FindControl(_T("autowatch"));
					}
					else if (userData >= 18 && userData < 25) {
						pFocusedItem = m_pManager->FindControl(_T("videoget"));
					}

					if (pFocusedItem) {
						SetItemBkColor(pFocusedItem,0xFFE6E6EF,0xFFFFFFFF);
						pFocusedItem->SetFocus();
					}
				}
				break;
			//----------------------------------------------------------------------

			case VK_RIGHT:
				int userdata = StrToInt(GetUserData());
				int sel;
				CTabLayoutUI *pThirdMenu = static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")));
				CVerticalLayoutUI *pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(pThirdMenu->GetCurSel());
				//报警视频列表
				if (userdata >= 0 && userdata < 6){
					CDuiString name;
					CListUI *pList;
					name.Format(_T("video_alarmlist%d"), userdata + 1);
					pList = static_cast<CListUI*>(m_pManager->FindControl(name));
					int Count = pList->GetCount();
					if (Count > 0){
						pList->GetItemAt(0)->SetFocus();
						pList->SelectItem(0);
					}
					else{
						return;
					}
				}
				else if (userdata == 6){
					//主机名称
					m_pManager->FindControl(_T("edit_shipname"))->SetFocus();
				}
				else if (userdata >= 7 && userdata < 13){
					//摄像机设置
					CButtonUI *pEdit;
					sel = pThirdMenu->GetCurSel();
					pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
					pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
					pEdit = (CButtonUI*)pChildLayout->GetItemAt(2); //船名编辑框
					pEdit->SetFocus();
				}
				else if (userdata == 13){
					//系统设置
					pChildLayout->GetItemAt(2)->SetFocus();
				}
				else if (userdata == 14){
					//看船时间
					static_cast<CButtonUI*>(m_pManager->FindControl(_T("time1_hour")))->SetFocus();
				}
				else if (userdata == 15){
					//报警音
					CButtonUI *pAlarmVoiceSwitch;
					sel = pThirdMenu->GetCurSel();
					pChildLayout = (CVerticalLayoutUI*)pThirdMenu->GetItemAt(sel);
					pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
					pAlarmVoiceSwitch = (CButtonUI*)pChildLayout->GetItemAt(2); //报警音开关
					pAlarmVoiceSwitch->SetFocus();
				}
				else if (userdata == 16){
					//警告灯光
					m_pManager->FindControl(_T("alarmlight_switch"))->SetFocus();
				}
				else if (userdata == 17){
					//看船开关记录
				}
				else if (userdata >= 18 && userdata < 24) {
					//视频列表
					CDuiString name;
					CListUI *pList;
					name.Format(_T("video_list%d"), userdata - 17);
					pList = static_cast<CListUI*>(m_pManager->FindControl(name));
					int Count = pList->GetCount();
					if (Count > 0) {
						pList->GetItemAt(0)->SetFocus();
						pList->SelectItem(0);
					}
					else {
						return;
					}
				}
				SetItemBkColor(NULL,0xFFEFEFF4, 0xFFFFFFFF);
				break;
		}
	}

	if (event.Type == UIEVENT_SETFOCUS) {
		CContainerUI *layout = (CContainerUI*)GetParent();
		if (layout->GetBkColor() == 0xFFEFEFF4)
		{
			layout->SetBkColor(0xFFFFFFFF);
			for (int i = 0; i < layout->GetCount(); i += 2)
			{
				layout->GetItemAt(i)->SetBkColor(0xFFFFFFFF);
			}
			SetTextColor(0xFF666666);
		}
		SetBkColor(0xFF4198FE);
		int i = StrToInt(GetUserData());
		static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SelectItem(StrToInt(GetUserData()));
	}
	else if(event.Type == UIEVENT_KILLFOCUS){
		if(GetParent()->GetBkColor()==0xFFFFFFFF)
			SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}

LPCTSTR CMenuItemUI::GetClass()
{
	return _T("MenuItem");
}

void CMenuItemUI::FindRecordFile()
{
	UINT8 RecordType;
	int inx,num_record,level;
	list<CRecordFileInfo*> recordInfo;
	CVideoListUI *pList=NULL;
	CMyListUI *pItem=NULL;
	CDuiString Listname;
	CVideoListUI::Node *node;
	inx = StrToInt(GetUserData());
	if (inx >= 0 && inx < 6) { //报警视频
		Listname.Format(_T("video_alarmlisti%d"), inx+1);
		pList = (CVideoListUI*)m_pManager->FindControl(Listname);
		num_record = pList->GetCount();
		RecordType = COPYING_ALARM;
	}
	else if (inx > 17 && inx < 24) { //正常视频
		Listname.Format(_T("video_list%d"), inx - 17);
		pList = (CVideoListUI*)m_pManager->FindControl(Listname);
		num_record = pList->GetCount();
		RecordType = COPYING_NORMAL;
	}
	for (int i = 0; i < num_record; i++) {
		pItem = (CMyListUI*)pList->GetItemAt(i);
		node = (CVideoListUI::Node*)pItem->GetTag();
		level = node->data()._level;
		if (level == 1){
			recordInfo.push_back(pItem->Info);
		}
	}
	HardDriverStatus(recordInfo, RecordType);
}

void CMenuItemUI::HardDriverStatus(list<CRecordFileInfo*> recordInfo, UINT8 RecordType)
{
	CExHardDrive* pHardDriver = CExHardDrive::GetInstance();
	USBFlashDiskStatus flashSize;
	ULONGLONG totalSize=0;
	//拷贝视频的总大小
	list<CRecordFileInfo*>::iterator iter;
	for (iter = recordInfo.begin(); iter != recordInfo.end(); iter++) {
		totalSize += (*iter)->dlSize;
	}

	//U盘已插入
	if(pHardDriver->IsInsert()){
		pHardDriver->GetStatus(&flashSize);
		if (flashSize.mSpaceLeft > totalSize) {
			CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_copyvideo_request.xml"), GetText(), NULL, NULL, NULL);
			if (!recordInfo.empty())
				SendMessage(m_pManager->GetPaintWindow(), USER_MSG_MESSAGE_BOX, RecordType, (LPARAM)(&recordInfo));
		}
		//空间不足
		else {
			CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_ok.xml"), _T("U盘没有足够的空间,请清除空间"), _T("或者更换U盘后，重试！"), NULL, NULL);
		}
	}
	else {
		CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_ok.xml"), NULL, _T("未检测到U盘，请重试！"), NULL, NULL);
	}
}
