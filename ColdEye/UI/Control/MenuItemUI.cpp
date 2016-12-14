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
	SetText(text);

	pParent->Add(this);
	SetAttribute(_T("style"), _T("MenuItem"));

	CControlUI * p = new CControlUI;
	pParent->Add(p);
	p->SetAttribute(_T("style"), _T("control"));
}


CMenuItemUI::CMenuItemUI(CContainerUI* pParent, CString& text, CDuiString userdata, int iInx)
{
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
		if (GetKeyState(VK_CONTROL) && !(event.wParam & 0x20000000)) {
			if (event.wParam=='U') {
				CDuiString userdata = GetUserData();
				int id = StrToInt(userdata);
				Print("id :%d",id);
				if (id >= 0 && id < 6) {
					
					UINT8 type;
					RecordInfo = FindRecordFile(&type);
					m_pManager->SendNotify(this, DUI_MSGTYPE_COPYFILE, type, (LPARAM)&RecordInfo);
				}
				else if (id >= 18 && id < 23) {
					UINT8 type;
					RecordInfo = FindRecordFile(&type);
					m_pManager->SendNotify(this, DUI_MSGTYPE_COPYFILE, type, (LPARAM)&RecordInfo);
				}
				
			}
		}

		m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);

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
		Print("userid:%d",i);
		static_cast<CTabLayoutUI*>(m_pManager->FindControl(_T("layout_thirdmenu")))->SelectItem(StrToInt(GetUserData()));
	}
	else if(event.Type == UIEVENT_KILLFOCUS){
		//if(GetParent()->GetBkColor()==0xFFFFFFFF)
		SetBkColor(0xFFFFFFFF);
	}

	CButtonUI::DoEvent(event);
}

LPCTSTR CMenuItemUI::GetClass()
{
	return _T("MenuItem");
}

list<CRecordFileInfo*> CMenuItemUI::FindRecordFile(UINT8* type)
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
		Listname.Format(_T("video_alarmlist%d"), inx+1);
		pList = (CVideoListUI*)m_pManager->FindControl(Listname);
		num_record = pList->GetCount();
		*type = 1;
	}
	else if (inx > 17 && inx < 24) { //正常视频
		Listname.Format(_T("video_list%d"), inx - 17);
		pList = (CVideoListUI*)m_pManager->FindControl(Listname);
		num_record = pList->GetCount();
		*type = 0;
	}
	for (int i = 0; i < num_record; i++) {
		pItem = (CMyListUI*)pList->GetItemAt(i);
		node = (CVideoListUI::Node*)pItem->GetTag();
		level = node->data()._level;
		if (level == 1){
			recordInfo.push_back(pItem->Info);
		}
	}
	return recordInfo;

	//HardDriverStatus(recordInfo, RecordType);
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
