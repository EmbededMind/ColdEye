#include "stdafx.h"
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

#include "Control\PopupMenuUI.h"

#include "Pattern\MsgSquare.h"
 
#include "Database\DBShadow.h"
#include "Database\DBLogger.h"

#include "Com\RecordAlarmSound.h"
#include "Com\MCI.h"

CDuiString  _PortName[24] = {  //对应名字id
	_T("摄像机1"),
	_T("摄像机2"),
	_T("摄像机3"),
	_T("摄像机4"),
	_T("摄像机5"),
	_T("摄像机6"),
	_T("船头摄像机"),
	_T("船甲板摄像机"),
	_T("船左舷摄像机"),
	_T("船右舷摄像机"),
	_T("船后方"),
	_T("船缆绳"),
	_T("集控台"),
	_T("监控台"),
	_T("船长室"),
	_T("船员室"),
	_T("主机舱"),
	_T("发电机舱"),
	_T("罗经甲板"),
	_T("一层甲板"),
	_T("二层甲板"),
	_T("三层甲板"),
	_T("四层甲板"),
	_T("五层甲板")
};

CMyMenuWnd::CMyMenuWnd()
	:focusLevel(0)
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
	return _T("Menu.xml");
}

void CMyMenuWnd::InitWindow()
{
	pLayout_PopMenu = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_popmenu")));
	pLayout_Menuitem = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_menuitem")));
	pLayout_third = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	pShipName = static_cast<CMyEditUI*>(m_pm.FindControl(_T("edit_shipname")));
	std::string& tmp = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.boat_name;
	CString text;
	text.Format(_T("%S"), tmp.c_str());
	pShipName->SetText(text);

	pKeyBoard = static_cast<CContainerUI*>(m_pm.FindControl(_T("keyboard")));

	pSysLight = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_light")));
	pSysLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness);

	pSysVolum = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_voice")));
	pSysVolum->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn);

	pAlmVicSwitch = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmvoice_switch")));
	pDefaultVoice = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("alarmvoice_default")));
	pVoice1 = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("voice1")));

	pAlarmLight = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmlight_switch")));
	pAlarmLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff);

	pAwOnOffRecordList = static_cast<CListUI*>(m_pm.FindControl(_T("watch_record")));
	pPage = static_cast<CLabelUI*>(m_pm.FindControl(_T("page")));
	pHomeWatch = static_cast<CButtonUI*>(m_pm.FindControl(_T("button_home")));
	if (pHomeWatch) pHomeWatch->OnNotify += MakeDelegate(this, &CMyMenuWnd::OnHomeWatch);
	//-------------------------看船时间控件关联---------------------------------
	pAwTime[0] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_hour")));
	pAwTime[1] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_minute")));
	pAwTime[2] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_hour")));
	pAwTime[3] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_minute")));

	pAwTime[0]->SetItemRelation(NULL, pAwTime[1]);
	pAwTime[1]->SetItemRelation(pAwTime[0], pAwTime[2]);
	pAwTime[2]->SetItemRelation(pAwTime[1], pAwTime[3]);
	pAwTime[3]->SetItemRelation(pAwTime[2], NULL);

	

	//-------------------------摄像机设置界面添加-------------------------------
	CDialogBuilder Camera1builder, Camera2builder, Camera3builder, Camera4builder, Camera5builder, Camera6builder;
	CVerticalLayoutUI *CamareChildLayout;
	CamareChildLayout = (CVerticalLayoutUI*)Camera1builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 7);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头1设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera2builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 8);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头2设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera3builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 9);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头3设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera4builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 10);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头4设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera5builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 11);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头5设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera6builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, 12);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头6设置"));

	
	DWORD tBegining  = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_begining;
	DWORD  tEnd  = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_end;

	SetWatchTime(tBegining, tEnd);

	CMsgSquare* pSquare = CMsgSquare::GetInstance();
	if (pSquare != NULL) {
		pSquare->AddAudience(m_hWnd, USER_MSG_LOGIN);
		pSquare->AddAudience(m_hWnd, USER_MSG_LOGOFF);
		pSquare->AddAudience(m_hWnd, USER_MSG_INITFILE);
		pSquare->AddAudience(m_hWnd, USER_MSG_ADDFILE);
	}
	InitAlarmVoice();
	InitAwOnOffRecord();
}


bool CMyMenuWnd::OnHomeWatch(void * param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	//Print("home message :%S", pMsg->sType);
	//if (pMsg->sType == DUI_MSGTYPE_CLICK) {
	//	m_pm.FindControl(_T("layout_home_watch"))->SetVisible(false);
	//	m_pm.FindControl(_T("notice"))->SetVisible(true);
	//}
	return false;
}

void CMyMenuWnd::UpdataItemColor()
{
	Print("focusLevel:%d ",focusLevel);
	if (focusLevel == 0) {
		//布局
		pLayout_PopMenu->SetBkColor(LAYOUT_POP_FOCUSED);
		pLayout_Menuitem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);

		//一级菜单
		CButtonUI* pItem;
		for (int i = 0; i < pLayout_PopMenu->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout_PopMenu->GetItemAt(i);
			if(pItem != FocusedItem[0])
				pItem->SetBkColor(LAYOUT_POP_FOCUSED);
		}
		FocusedItem[0]->SetTextColor(0xFF666666);
		FocusedItem[0]->SetFocusedTextColor(0xFFFFFFFF);
		//二级菜单
		CVerticalLayoutUI* pLayout = (CVerticalLayoutUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel());
		for (int i = 0; i < pLayout->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);
		}
	}
	else if (focusLevel==1) {
		//布局
		pLayout_PopMenu->SetBkColor(LAYOUT_POP_NOFOCUS);
		pLayout_Menuitem->SetBkColor(LAYOUT_MENUITEM_FOCUSED);

		//一级菜单
		CButtonUI* pItem;
		for (int i = 0; i < pLayout_PopMenu->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout_PopMenu->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_POP_NOFOCUS);
		}
		FocusedItem[0]->SetBkColor(POPMENU_SEL_NOFOCUS);
		FocusedItem[0]->SetTextColor(0xFFFFFFFF);
		//二级菜单
		CVerticalLayoutUI* pLayout = (CVerticalLayoutUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel());
		for (int i = 0; i < pLayout->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_MENUITEM_FOCUSED);
		}
		m_pm.GetFocus()->SetBkColor(ITEM_FOCUS);
		static_cast<CButtonUI*>(m_pm.GetFocus())->SetTextColor(0xFF666666);
		static_cast<CButtonUI*>(m_pm.GetFocus())->SetFocusedTextColor(0xFFFFFFFF);
	}
	else if(focusLevel==2){
		//布局
		pLayout_Menuitem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);
		pLayout_third->SetBkColor(LAYOUT_THIRD_FOCUSED);
		CButtonUI* pItem;
		CVerticalLayoutUI* pLayout = (CVerticalLayoutUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel());
		for (int i = 0; i < pLayout->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);
		}
		FocusedItem[1]->SetBkColor(MENUITEM_SEL_NOFOCUS);
		FocusedItem[1]->SetTextColor(0xFFFFFFFF);
	}
}



void CMyMenuWnd::SliderNotify(TNotifyUI & msg)
{
	CDuiString sName = msg.pSender->GetName();
	CMySliderUI *Item = (CMySliderUI*)msg.pSender;
	switch (msg.wParam) {
	case VK_UP:
		if (sName == _T("camera_set_volume")) {
			CVerticalLayoutUI *pParentLayout = (CVerticalLayoutUI*)msg.pSender->GetParent();
			pParentLayout->GetItemAt(1)->SetFocus();
		}
		else if (sName == _T("sysset_voice")) {
			m_pm.FindControl(_T("sysset_light"))->SetFocus();
		}
		break;
		//------------------------------------------------------
	case VK_DOWN:
		if (sName == _T("sysset_light")) {
			m_pm.FindControl(_T("sysset_voice"))->SetFocus();
		}
		else if (sName == _T("sysset_voice")) {
			m_pm.FindControl(_T("sysset_version"))->SetFocus();
		}
		else if (sName == _T("camera_set_volume")) {
			CVerticalLayoutUI *pParentLayout = (CVerticalLayoutUI*)Item->GetParent();
			pParentLayout->GetItemAt(6)->SetFocus();
		}
		break;
		//-------------------------------------------------------
	case VK_BACK:
		
		if (sName == _T("camera_set_volume")) {
			CameraInfo info;
			info.Volumn  = 1;

			CMenuItemUI* pMenuItem  = (CMenuItemUI*)pLayout_third->GetItemAt(7);
			::SendMessage( ((CColdEyeDlg*)AfxGetMainWnd())->m_hWnd,  USER_MSG_CAMERA_CONFIG_CHANGE, DEV_CONFIG_MASK_VOL, (LPARAM)NULL);			
		}
		break;
	}
}

void CMyMenuWnd::EditNotify(TNotifyUI & msg)
{
	CDuiString sName = msg.pSender->GetName();
	CMyEditUI *pItem = (CMyEditUI*)msg.pSender;
	switch (msg.wParam) {
	case VK_DOWN:
		if (sName == _T("edit_cameraname")) {
			CVerticalLayoutUI *pVerLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(pLayout_third->GetCurSel());
			CVerticalLayoutUI *pBottomLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //最底下布局
			pBottomLayout->GetItemAt(1)->SetFocus();
		}
		break;
	//-----------------------------------------------
	case VK_RETURN:
		pItem->SetStatus(true);
		if (sName == _T("edit_shipname")) {
			pKeyBoard->SetVisible(true);
			m_pm.FindControl(_T("prompt"))->SetVisible(true); //提示信息
			m_pm.FindControl(_T("vk_1"))->SetFocus();
		}
		else if (sName == _T("edit_cameraname")) {
			ExpandCameraName();
		}
		break;
	}
}

void CMyMenuWnd::MenuItemNotify(TNotifyUI & msg)
{
	CButtonUI *pItem = (CButtonUI*)msg.pSender;
	CContainerUI *pLayout = (CContainerUI*)pItem->GetParent();
	CContainerUI *pChildLayout;
	CDuiString userdata = msg.pSender->GetUserData();
	CContainerUI *pNextFocusLayout;
	int ItemIndex = pLayout->GetItemIndex(pItem); //控件的排序
	if (pLayout == pLayout_PopMenu) {
		switch (msg.wParam) {
		case VK_UP:
			if (ItemIndex > 0) {
				pLayout->GetItemAt(ItemIndex - 2)->SetFocus();
			}
			break;
		//-----------------------------------------------
		case VK_DOWN:			
			if (pLayout->GetItemAt(ItemIndex + 2)) {
				pLayout->GetItemAt(ItemIndex + 2)->SetFocus();
			}
			break;
		//-----------------------------------------------
		case VK_RIGHT:
			FocusedItem[0] = pItem; 
			pNextFocusLayout = (CContainerUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel()); //下一集焦点的布局
			if (userdata == _T("4")) {
				//pHomeWatch->SetFocus();
			}
			else if (pNextFocusLayout->GetCount() > 0) {
				focusLevel++;
				pLayout_third->SetVisible(true);
				pNextFocusLayout->GetItemAt(0)->SetFocus();

				UpdataItemColor();
			}
			break;
		//-----------------------------------------------
		}
	}
	else {
		switch (msg.wParam) {
		case VK_UP:
			if (ItemIndex > 0) {
				pLayout->GetItemAt(ItemIndex - 2)->SetFocus();
			}
			break;
			//-----------------------------------------------
		case VK_DOWN:
			if (pLayout->GetItemAt(ItemIndex + 2)) {
				pLayout->GetItemAt(ItemIndex + 2)->SetFocus();
			}
			break;
			//-----------------------------------------------
		case VK_LEFT:
			FocusedItem[0]->SetFocus();
			FocusedItem[1] = NULL;
			focusLevel--;
			pLayout_third->SelectItem(0);
			pLayout_third->SetVisible(false);
			UpdataItemColor();
			break;
			//-----------------------------------------------
		case VK_RIGHT:
			FocusedItem[1] = pItem;
			ThirdMenuSetFocus(userdata);
			if (m_pm.GetFocus() != FocusedItem[1]) {
				focusLevel++;
				UpdataItemColor();
			}
			break;
		}
	}
}

void CMyMenuWnd::LabelNotify(TNotifyUI & msg)
{
	CDuiString sName = msg.pSender->GetName();
	CMyEditUI *pItem = (CMyEditUI*)msg.pSender;
	CVerticalLayoutUI *pParentLayout = (CVerticalLayoutUI*)pItem->GetParent();
	switch (msg.wParam) {
	case VK_DOWN:
		if (sName == _T("camera_set_video_save") || sName == _T("sysset_version")\
			|| sName == _T("sysset_reset") || sName == _T("sysset_host_model")
			) {
			pParentLayout->GetItemAt(pParentLayout->GetItemIndex(pItem) + 2)->SetFocus();
		}
		break;
	//--------------------------------------
	case VK_UP:
		pParentLayout->GetItemAt(pParentLayout->GetItemIndex(pItem) - 2)->SetFocus();
		break;

	}
}

void CMyMenuWnd::CopyFileNotify(TNotifyUI & msg)
{
	//未检测到U盘
	int HardDriveStatus = DetectHardDriver((list<CRecordFileInfo*>*)msg.lParam);
	if (HardDriveStatus == 0) {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), NULL, _T("未检测到U盘，请重试！"), NULL, NULL);
	}
	//U盘正常
	else if (HardDriveStatus == 1) {
		PrepareCopy((list<CRecordFileInfo*>*)msg.lParam, msg.wParam);
	}
	//U盘空不足
	else {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), _T("U盘没有足够的空间, 请清除空间"), _T("或者更换U盘后，重试！"), NULL, NULL);
	}
}

void CMyMenuWnd::RecordVoiceNotify(TNotifyUI & msg)
{
	switch (msg.wParam) {
	case VK_RETURN:
		RecordVoice();
		break;

	case VK_UP:
		if (pVoice1) {
			pVoice1->SetFocus();
		}
		else {
			pDefaultVoice->SetFocus();
		}
		break;
	}
}


void CMyMenuWnd::AlarmVoiceListNotify(TNotifyUI & msg)
{
	CAlarmVoiceListUI* pSend = (CAlarmVoiceListUI*)msg.pSender;
	CDuiString sName = msg.pSender->GetName();
	switch (msg.wParam) {
	case VK_UP:
		if (pSend == pVoice1) {
			pDefaultVoice->SetFocus();
		}
		else {
			pAlmVicSwitch->SetFocus();
		}
		break;

	case VK_DOWN:
		if (pSend == pDefaultVoice) {
			if (pVoice1) {
				pVoice1->SetFocus();
			}
			else {
				m_pm.FindControl(_T("record_btn"))->SetFocus();
			}
		}
		else {
			m_pm.FindControl(_T("record_btn"))->SetFocus();
		}
		break;

	case VK_RETURN:
		if (pSend == pDefaultVoice) {
			mAlarmVoiceSel = 0;
			pDefaultVoice->SetVoiceSel(true);
			if(pVoice1)
				pVoice1->SetVoiceSel(false);
		}
		else {
			mAlarmVoiceSel = 1;
			pDefaultVoice->SetVoiceSel(false);
			pVoice1->SetVoiceSel(true);
		}
		m_pm.Invalidate();
		break;
	}
}

void CMyMenuWnd::SwitchNotify(TNotifyUI & msg)
{
	CSwitchUI* pItem = (CSwitchUI*)msg.pSender;
	CDuiString sName = pItem->GetName();
	switch (msg.wParam){
	case VK_LEFT:
		if (pItem->GetValue()) {
			pItem->SetValue(false);
			if (pItem == pAlmVicSwitch) {
				ShowAlarmVoiceList(pItem->GetValue());
			}
			pItem->Invalidate();
		}
		break;
	case VK_RIGHT:
		if (!pItem->GetValue()) {
			pItem->SetValue(true);
			if (pItem == pAlmVicSwitch) {
				ShowAlarmVoiceList(pItem->GetValue());
			}
			pItem->Invalidate();
		}
		break;
	case VK_UP:
		if (_tcscmp(sName, _T("camera_switch")) == 0) {
			int inx = pLayout_third->GetCurSel() - CAMERA_SET;
			camera[inx].pShipname->SetFocus();
		}
		break;

	case VK_DOWN:
		if (_tcscmp(sName, _T("camera_switch")) == 0) {
			int inx = pLayout_third->GetCurSel() - CAMERA_SET;
			camera[inx].pVolum->SetFocus();
		}
		else if (pItem == pAlmVicSwitch) {
			if (pItem->GetValue()) {
				pDefaultVoice->SetFocus();
			}
		}
		break;
	}
}

void CMyMenuWnd::ListLabelNotify(TNotifyUI & msg)
{
	if (GetKeyState(VK_CONTROL) && !(msg.wParam & 0x20000000)) {
		if (msg.wParam == 'L') { //加锁
			if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ListLabelElementUI")) == 0){
				CMyListUI *pSender = (CMyListUI*)msg.pSender;
				if (pSender->Info->status == RECORD_LOCKED) {
					pSender->Info->status = RECORD_SEEN;
				}
				else {
					if (pSender->Info->status == RECORD_NSEEN) {
						refreshSuperscript(pSender);
					}
					pSender->Info->status = RECORD_LOCKED;
				}
				pSender->Invalidate();
			}

		}
	}

	switch (msg.wParam) {
	case VK_BACK:
		if (FocusedItem[1]) {
			CVideoListUI* list = (CVideoListUI*)m_pm.GetFocus()->GetParent()->GetParent();
			list->UnSelectAllItems();
			FocusedItem[1]->SetFocus();
			FocusedItem[1] = NULL;
			focusLevel--;
			UpdataItemColor();
		}
		break;

	case VK_RIGHT:
		AwOnOffRecordNextPage();
		break;
		
	case VK_LEFT:
		AwOnOffRecordLastPage();
		break;
		
	case VK_UP: {
			int sel=0;
			sel = pAwOnOffRecordList->GetCurSel();
			if (sel == 0) {
				AwOnOffRecordLastPage();
			}
			else {
				pAwOnOffRecordList->SelectItem(sel - 1);
			}
		}
		break;

	case VK_DOWN: {
			int sel = 0;
			sel = pAwOnOffRecordList->GetCurSel();
			if (sel == 12) {
				AwOnOffRecordNextPage();
			}
			else if(sel != pAwOnOffRecordList->GetCount()-1){
				pAwOnOffRecordList->SelectItem(sel + 1);
			}
		}
		break;
	default:
		break;
	}
}

void CMyMenuWnd::ExpandCameraName()
{
	CVerticalLayoutUI *pLayout;
	CVerticalLayoutUI *pChildLayout1;
	CVerticalLayoutUI *pChildLayout2;
	CContainerUI *pChildItem;

	CRect rect;
	CSize size;
	pLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(pLayout_third->GetCurSel());

	pChildLayout1 = (CVerticalLayoutUI*)pLayout->GetItemAt(1);
	pChildLayout2 = (CVerticalLayoutUI*)pLayout->GetItemAt(2);
	if (pChildLayout1->IsVisible() == false) {
		rect = pChildLayout2->GetPos();
		size.cx = 70;
		size.cy = rect.top + 204;
		pChildLayout2->SetFixedXY(size);
		pChildLayout1->SetVisible(true);
		pChildItem = (CContainerUI*)pChildLayout1->GetItemAt(0);
		pChildItem = (CContainerUI*)pChildItem->GetItemAt(0);
		pChildItem = (CContainerUI*)pChildItem->GetItemAt(0);
		pChildItem->SetFocus();
		CMyEditUI *pItem = (CMyEditUI*)(static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(0))->GetItemAt(2));
		pItem->SetStatus(true);
	}
	else {
		pChildLayout1->SetVisible(false);
		rect = pChildLayout2->GetPos();
		size.cx = 70;
		size.cy = rect.top - 204;
		pChildLayout2->SetFixedXY(size);
		CMyEditUI *pItem = (CMyEditUI*)(static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(0))->GetItemAt(2));
		pItem->SetFocus();
		pItem->SetStatus(false);
	}
}

void CMyMenuWnd::ShowAlarmVoiceList(bool Switch)
{
	CContainerUI* pContain = (CContainerUI*)m_pm.FindControl(_T("alarm_voice"));
	pContain->SetVisible(Switch);
}

void CMyMenuWnd::ThirdMenuSetFocus(CDuiString userdata)
{
	int inx = StrToInt(userdata);
	int sel;
	CVerticalLayoutUI *pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(pLayout_third->GetCurSel());
	//报警视频列表
	if (inx >= 0 && inx < 6){
		CDuiString name;
		CListUI *pList;
		name.Format(_T("video_alarmlist%d"), inx + 1);
		pList = static_cast<CListUI*>(m_pm.FindControl(name));
		int Count = pList->GetCount();
		if (Count > 0){
			pList->GetItemAt(0)->SetFocus();
			pList->SelectItem(0);
		}
		else{
			return;
		}
	}
	else if (inx == 6){
		//主机名称
		m_pm.FindControl(_T("edit_shipname"))->SetFocus();
	}
	else if (inx >= 7 && inx < 13){
		//摄像机设置
		CButtonUI *pEdit;
		sel = pLayout_third->GetCurSel();
		pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(sel);
		pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
		pEdit = (CButtonUI*)pChildLayout->GetItemAt(2); //船名编辑框
		pEdit->SetFocus();
	}
	else if (inx == 13){
		//系统设置
		pChildLayout->GetItemAt(2)->SetFocus();
	}
	else if (inx == 14){
		//看船时间
		static_cast<CButtonUI*>(m_pm.FindControl(_T("time1_hour")))->SetFocus();
	}
	else if (inx == 15){
		//报警音
		CButtonUI *pAlarmVoiceSwitch;
		sel = pLayout_third->GetCurSel();
		pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(sel);
		pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
		pAlarmVoiceSwitch = (CButtonUI*)pChildLayout->GetItemAt(2); //报警音开关
		pAlarmVoiceSwitch->SetFocus();
	}
	else if (inx == 16){
		//警告灯光
		m_pm.FindControl(_T("alarmlight_switch"))->SetFocus();
	}
	else if (inx == 17){
		//看船开关记录
		if (pAwOnOffRecordList->GetCount() > 0) {
			pAwOnOffRecordList->GetItemAt(0)->SetFocus();
			pAwOnOffRecordList->SelectItem(0);
		}
	}
	else if (inx >= 18 && inx < 24) {
		//视频列表
		CDuiString name;
		CListUI *pList;
		name.Format(_T("video_list%d"), inx - 17);
		pList = static_cast<CListUI*>(m_pm.FindControl(name));
		int Count = pList->GetCount();
		if (Count > 0) {
			pList->GetItemAt(0)->SetFocus();
			pList->SelectItem(0);
		}
		else {
			return;
		}
	}
}

int CMyMenuWnd::DetectHardDriver(list<CRecordFileInfo*>* recordInfo)
{
	CExHardDrive* pHardDriver = CExHardDrive::GetInstance();
	USBFlashDiskStatus flashSize;
	ULONGLONG totalSize = 0;
	//拷贝视频的总大小
	list<CRecordFileInfo*>::iterator iter;
	for (iter = recordInfo->begin(); iter != recordInfo->end(); iter++) {
		totalSize += (*iter)->dlSize;
	}

	//U盘已插入
	if (pHardDriver->IsInsert()) {
		//if (recordInfo->empty()) return;
		pHardDriver->GetStatus(&flashSize);
Print("flashSize:%llu",flashSize.mSpaceLeft);
Print("totalSize:%llu",totalSize);
		if (flashSize.mSpaceLeft > totalSize) {
			return 1;
		}
		//空间不足
		else {
			return 2;
		}
	}
	else {
		return 0;
	}
}

void CMyMenuWnd::PrepareCopy(list<CRecordFileInfo*>*recordInfo, UINT8 type)
{
	CControlUI *focusedItem = NULL;
	CDuiString text;
	if (recordInfo->empty()) return;
	CDuiString ClassName;
	focusedItem = m_pm.GetFocus();
	ClassName = focusedItem->GetClass();
	//焦点在MenuItem上 按复制
	if (ClassName == _T("ButtonUI")) {
		text = focusedItem->GetText();
		CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo_request.xml"), text, NULL, NULL, NULL);
	}
	//焦点在文件列表位置时，点击复制
	else {
		CMyListUI *ListItem = (CMyListUI*)focusedItem;
		CVideoListUI::Node* pNode;
		text = focusedItem->GetText();
		pNode = (CVideoListUI::Node*)static_cast<CMyListUI*>(focusedItem)->GetTag();
		Print("lel:%d", pNode->data()._level);
		if (pNode->data()._level == 0) {
			text = focusedItem->GetText();
		}
		else {
			CTime tbegin = CTime(ListItem->Info->tBegin);
			CTime tend = CTime(ListItem->Info->tEnd);
			text = tbegin.Format("%m月%d日   ") + tbegin.Format("%H:%M:%S") + _T("-") + tend.Format("%H:%M:%S");
		}
		CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo_request.xml"), text, NULL, NULL, NULL);
	}
	if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo.xml"), text, NULL, (LPARAM)recordInfo, type)) {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), NULL, _T("复制成功！"), NULL, NULL);
	}

}

void CMyMenuWnd::PlayVideo(WPARAM wParam,LPARAM lParam)
{
	mPlayerWall = new CPlayerWallWnd(_T("playerwall.xml"));
	mPlayerWall->Create(NULL, _T("PlayerWallWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, { 0,0,0,0 });
	mPlayerWall->ShowWindow(true);
	mPlayerWall->CenterWindow();
	::SendMessage(mPlayerWall->GetHWND(), USER_MSG_PLAY_START, wParam, lParam);
}

void CMyMenuWnd::RecordVoice()
{
	int result;
	CDuiString text;
	text = static_cast<CButtonUI*>(m_pm.FindControl(_T("record_btn")))->GetText();
	CMsgWnd::MessageBox(m_hWnd, _T("mb_recordingvoice.xml"), NULL, NULL,NULL,NULL);
	result = CMsgWnd::MessageBox(this->GetHWND(), _T("mb_playvoice.xml"), text, NULL, NULL, NULL);
	if (MSGID_OK == result) {
		CMCI::GetInstance()->Save();
		CRecordAlarmSound::GetInstance()->Save();
		if(!pVoice1)
			AddAlarmVoice();
	}
	else if (MSGID_CANCEL == result) {
		CMCI::GetInstance()->NotSave();
		CRecordAlarmSound::GetInstance()->NotSave();
	}
	else {
		CMCI::GetInstance()->NotSave();
		CRecordAlarmSound::GetInstance()->NotSave();
		RecordVoice();
	}
}

void CMyMenuWnd::AddAlarmVoice()
{
	CButtonUI *pRecordBt = (CButtonUI*)m_pm.FindControl(_T("record_btn")); //录制按键
	CContainerUI *layout = (CContainerUI*)pRecordBt->GetParent();
	pVoice1 = new CAlarmVoiceListUI(_T("录音1"),_T("voice1"));
	SIZE size_bnt = pRecordBt->GetFixedXY();
	size_bnt.cy += 64;
	layout->SetFixedHeight(422);
	pRecordBt->SetFixedXY(size_bnt);
	pRecordBt->SetText(_T("重新录制报警音"));
	layout->Add(pVoice1);
	pVoice1->SetAttribute(_T("style"), _T("alarm_voice_list"));
	pVoice1->SetFixedXY({ 0,360 });
}

void CMyMenuWnd::refreshSuperscript(CMyListUI* pSender)
{
	CVideoListUI::Node* pNode;
	CMyListUI* pHead;
	pNode = (CVideoListUI::Node*)pSender->GetTag();
	pHead = pNode->parent()->data()._pListElement;

	if (pHead->mhintNumber > 0) {
		pHead->mhintNumber--;
		pHead->Invalidate();


		CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
		pPort->m_virginNumber -= 1;
		FocusedItem[1]->Invalidate();

		SetAllVirginNum();
	}

}

void CMyMenuWnd::Notify(TNotifyUI & msg)
{
	if (msg.sType == DUI_MSGTYPE_SLIDER) {
		SliderNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_EDIT) {
		EditNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_MENU) {
		MenuItemNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_LABEL) {
		LabelNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_PLAYER) {
		CMyListUI* pSender = (CMyListUI*)msg.pSender;
		if (pSender->Info->status == RECORD_NSEEN) {
			pSender->Info->status = RECORD_SEEN;
			refreshSuperscript(pSender);
		}
		PlayVideo(msg.wParam,msg.lParam);
	}
	else if (msg.sType == DUI_MSGTYPE_COPYFILE) {
		CopyFileNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_RECORDVOICE) {
		RecordVoiceNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_ALA_VOICE_LIST) {
		AlarmVoiceListNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_SWITCH) {
		SwitchNotify(msg);
	}
	else if (msg.sType == DUI_MSGTYPE_LISTLABEL) {
		ListLabelNotify(msg);
	}
}


LRESULT CMyMenuWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{ 
	switch (uMsg)
	{
	case USER_MSG_LOGIN: {
		 Print("Menu case login msg");

		 CDBShadow* pShadow = CDBShadow::GetInstance();

		 if (pShadow->GetAlarmFileNumber(((CPort*)lParam)->m_Id)  ==  0) {
			 AddAlarmMenuItem((CPort*)lParam);
		 }

		 if (pShadow->GetRecordFileNumber(((CPort*)lParam)->m_Id)  ==  0) {
			 AddVideoObtainMenuItem((CPort*)lParam);
		 }


		 AddPortConfigMenuItem((CPort*)lParam);
		 FillPortConfig((CPort*)lParam);
		 }

		 break;
		//-------------------------------------------
	case USER_MSG_LOGOFF: {
			Print("Menu case logoff msg");
			CDBShadow* pShadow  = CDBShadow::GetInstance();
			CPort*     pPort    = (CPort*)lParam;

			if (pShadow->GetAlarmFileNumber(pPort->GetId()) == 0) {
				DeleteAlarmMenuItem(pPort);
			}

			if (pShadow->GetRecordFileNumber(pPort->GetId()) == 0) {
				DeleteVideoObtainMenuItem(pPort);
			}

			DeletePortConfigMenuItem(pPort);
		}
			break;
		//-------------------------------------------
		case USER_MSG_INITFILE:
			if (wParam == RECORD_ALARM) {
				InitAlarmFile((list<CRecordFileInfo*>*)lParam);

				CDBShadow* pShadow = CDBShadow::GetInstance();
				CPortManager* pPortMgr = CPortManager::GetInstance();

				if (pShadow) {
					for (int i = 0; i < 6; i++) {
						if (pShadow->GetAlarmFileNumber(i + 1)) {
							CPort* pPort = pPortMgr->GetPortById(i+1);
							Print("Init add port:%d with %d files", pPort->m_Id, pShadow->GetAlarmFileNumber(i+1));
							AddAlarmMenuItem(pPort);
						}
					}
				}
			}
			else {
				InitRecordFile((list<CRecordFileInfo*>*)lParam);

				CDBShadow* pShadow = CDBShadow::GetInstance();
				CPortManager* pPortMgr = CPortManager::GetInstance();

				if (pShadow) {
					for (int i = 0; i < 6; i++) {
						if (pShadow->GetRecordFileNumber(i + 1)) {						
							CPort* pPort = pPortMgr->GetPortById(i+1);
							if (pPort) {
								AddVideoObtainMenuItem(pPort);
							}							
						}
					}
				}
			}
			break;
		//---------------------------------------------------------------------------
		case USER_MSG_ADDFILE:
			if (wParam == RECORD_ALARM) {
				CRecordFileInfo* pInfo = (CRecordFileInfo*)lParam;
				camera[pInfo->nOwner - 1].pAlarmList->AddRecordFile(pInfo);

				//port->virgin ++;
				CPort* port = (CPort*)pAlarmItem[pInfo->nOwner - 1]->GetTag();
				port->m_virginNumber++;
			}
			else {
				CRecordFileInfo* pInfo = (CRecordFileInfo*)lParam;
				Print("nOwner:%d", pInfo->nOwner);
				camera[pInfo->nOwner - 1].pNormalList->AddRecordFile(pInfo);
			}
			break;
        //--------------------------------------------
		case USER_MSG_DELFILE:
			if (wParam == RECORD_ALARM) {
				CRecordFileInfo* pInfo = (CRecordFileInfo*)lParam;
				camera[pInfo->nOwner - 1].pAlarmList->DeleteRecordFile(pInfo);

				// if pInfo is virgin  ,port->virgin--
				if (pInfo->status == RECORD_NSEEN) {
					CPort* port = (CPort*)pAlarmItem[pInfo->nOwner - 1]->GetTag();
					port->m_virginNumber--;
				}
			}
			else {
				CRecordFileInfo* pInfo = (CRecordFileInfo*)lParam;
				camera[pInfo->nOwner - 1].pNormalList->DeleteRecordFile(pInfo);
			}
			break;
		//--------------------------------------------
		case WM_KEYDOWN: 
			
			break;
		//---------------------------------------------------------------
		case USER_MSG_CAMERA_CONFIG_AWTIME:
			DWORD aw_begining, aw_end;
			GetWatchTime(&aw_begining, &aw_end);
			if (((CColdEyeApp*)AfxGetApp())->SetAwTime(aw_begining, aw_end)) {
				MSG msg;
				msg.message = USER_MSG_CAMERA_CONFIG_AWTIME;
				CMsgSquare::GetInstance()->Broadcast(msg);
			}
			break;
		//-------------------------------------------------------------
	}
	return LRESULT();
}

void CMyMenuWnd::GetCameraItem(CVerticalLayoutUI * pLayout)
{
	CContainerUI  *pChildLayout;
	CDuiString name;
	int iInx;
	iInx =pLayout_third->GetItemIndex(pLayout)-7;
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(0));
	camera[iInx].pTitle = (CLabelUI*)pChildLayout->GetItemAt(0);		//标题
	camera[iInx].pShipname = (CMyEditUI*)pChildLayout->GetItemAt(2);	//船名
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(2));
	camera[iInx].pSwitch = (CSwitchUI*)pChildLayout->GetItemAt(1);//摄像头开关
	camera[iInx].pVolum = (CMySliderUI*)pChildLayout->GetItemAt(4);//音量
	camera[iInx].pSaveVideo = (CMyLabelUI*)pChildLayout->GetItemAt(6);//视频存储设置
	camera[iInx].pAutoWatch = (CMyLabelUI*)pChildLayout->GetItemAt(8);//自动看船设置
	name.Format(_T("video_list%d"), iInx + 1);
	camera[iInx].pNormalList = (CVideoListUI*)m_pm.FindControl(name);	//视频列表
	name.Format(_T("video_alarmlist%d"), iInx + 1);
	camera[iInx].pAlarmList = (CVideoListUI*)m_pm.FindControl(name);	//报警视频列表
}

void CMyMenuWnd::InitRecordFile(list<CRecordFileInfo*>* pList)
{
	list<CRecordFileInfo*>::iterator  iter;
	for (iter = pList->begin(); iter != pList->end(); iter++) {
		camera[(*iter)->nOwner - 1].pNormalList->AddRecordFile(*iter);
	}
}

void CMyMenuWnd::InitAlarmFile(list<CRecordFileInfo*>* pList)
{
	list<CRecordFileInfo*>::iterator iter;
	for (iter = pList->begin(); iter != pList->end(); iter++) {
		camera[(*iter)->nOwner - 1].pAlarmList->AddRecordFile(*iter);
	}
}

void CMyMenuWnd::SetAllVirginNum()
{
	int totalVirginNum = 0;
	for (int i = 0; i < 6; i++) {
		if (pAlarmItem[i]) {
			CPort* port = (CPort*)pAlarmItem[i]->GetTag();
			Print("Get %d port %d virgin file", port->GetId(), port->m_virginNumber);
			totalVirginNum += port->m_virginNumber;
		}
	}
	
	CPopupMenuUI* pAlarmVideo = static_cast<CPopupMenuUI*>(m_pm.FindControl(_T("alarmvideo")));
	CPort* pPort = (CPort*)pAlarmVideo->GetTag();
	if (!pPort) {
		 pPort = new CPort;
		 pAlarmVideo->SetTag((UINT_PTR)pPort);
	}
	pPort->m_virginNumber = totalVirginNum;
	pAlarmVideo->Invalidate();
}

void CMyMenuWnd::DeleteAlarmCtl(CameraInfo cameraInfo)
{
	DeleteCtl(cameraInfo, _T("layout_submenu_alarm"), ALARM_VIDEO);
	camera[cameraInfo.id].pAlarmList->RemoveAll();
	camera[cameraInfo.id].pAlarmList = NULL;
}

void CMyMenuWnd::DeleteCameraSetCtl(CameraInfo cameraInfo)
{
	DeleteCtl(cameraInfo, _T("layout_submenu_setting"), CAMERA_SET);
}

void CMyMenuWnd::DeleteVideoObtain(CameraInfo cameraInfo)
{
	DeleteCtl(cameraInfo, _T("layout_submenu_videoget"), VIDEO_OBTAIN);
	camera[cameraInfo.id].pNormalList->RemoveAll();
	camera[cameraInfo.id].pNormalList = NULL;
}


void CMyMenuWnd::InitAwOnOffRecord()
{
	pAwOnOffRecordList->RemoveAll();
	char sqlStmt[128];
	sprintf_s(sqlStmt, "SELECT * FROM log LIMIT 13 OFFSET 1;");
	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);
	CDuiString sOption,sPage;
	while (stmt->NextRow()) {
		CTime time = stmt->ValueInt(0);
		sOption = GetStringOption(stmt->ValueInt(1), stmt->ValueInt(2));
		AddAwOnOffRecord(time, sOption);
	}
	sprintf_s(sqlStmt, "SELECT COUNT(*) FROM log;");
	stmt = sqlite.Statement(sqlStmt);
	while (stmt->NextRow()) {
		mAwTotalPage = stmt->ValueInt(0);
	}
	if (mAwTotalPage % 13) {
		mAwTotalPage = mAwTotalPage / 13 + 1;
	}
	else {
		mAwTotalPage = mAwTotalPage / 13;
	}
	if(mAwTotalPage>0)
		mAwPage = 1;
	else mAwPage = 0;
	sPage.Format(_T("第%d/%d"), mAwPage, mAwTotalPage);
	pPage->SetText(sPage);
}

CDuiString CMyMenuWnd::GetStringOption(int option,int tag)
{
	switch (option) {
	case LOG_POWER_ONOFF: //系统上电
		if (tag) {
			return _T("系统上电");
		}
		else {
			return _T("系统断电");
		}
		break;

	case LOG_SYS_TIME:
		return _T("gg");
		break;

		//自动看船开关（全局）
	case LOG_AUTO_WATCH:
		if (tag) {
			return _T("自动看船开启");
		}
		else {
			return _T("自动看船关闭");
		}
		break;

		//摄像头开
	case LOG_CAM_ON:
		return _PortName[tag] + _T("开启");
		break;

		//摄像头关
	case LOG_CAM_OFF:
		return _PortName[tag] + _T("关闭");
		break;

		//摄像头自动看船开启
	case LOG_CAM_AWON:
		return _PortName[tag] + _T("自动看船开启");
		break;

		//摄像头自动看船关闭
	case LOG_CAM_AWOFF:
		return _PortName[tag] + _T("自动看船关闭");
		break;
	default:
		break;
	}
}

void CMyMenuWnd::AddAwOnOffRecord(CTime sTime, CDuiString sType)
{
	int Count;
	CAWOnOffListLabelUI *pListEle;
	pListEle = new CAWOnOffListLabelUI(sTime, sType);
	pAwOnOffRecordList->Add(pListEle);
	pListEle->SetAttribute(_T("style"), _T("watch_record_style"));
	Count = pAwOnOffRecordList->GetCount();
	if (Count % 2)
		pListEle->SetBkColor(0xFFF3F3F3);
	else
		pListEle->SetBkColor(0xFFE5E5E5);
}

void CMyMenuWnd::AwPage(int page)
{
	if (page <= mAwTotalPage && page>0) {
		mAwPage = page;
		pAwOnOffRecordList->RemoveAll();
		char sqlStmt[128];
		sprintf_s(sqlStmt, "SELECT * FROM log LIMIT 13 OFFSET %d;", (page - 1) * 13 + 1);
		SQLiteStatement* stmt = sqlite.Statement(sqlStmt);
		CDuiString sOption, sPage;
		while (stmt->NextRow()) {
			CTime time = stmt->ValueInt(0);
			sOption = GetStringOption(stmt->ValueInt(1), stmt->ValueInt(2));
			AddAwOnOffRecord(time, sOption);
		}
		pAwOnOffRecordList->GetItemAt(0)->SetFocus();
		pAwOnOffRecordList->SelectItem(0);
		sPage.Format(_T("第%d/%d"), page, mAwTotalPage);
		pPage->SetText(sPage);
	}
}

void CMyMenuWnd::AwOnOffRecordNextPage()
{
	if (mAwPage < mAwTotalPage) {
		mAwPage++;
		AwPage(mAwPage);
	}
}

void CMyMenuWnd::AwOnOffRecordLastPage()
{
	if (mAwPage > 1) {
		mAwPage--;
		AwPage(mAwPage);
	}
}

int CMyMenuWnd::InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData)
{
	int order=0;
	CDuiString  userdata;
	UINT8 Count,CtlId;

	Count = pLayout->GetCount() / 2;
	for (int i = 0; i < Count; i+=2) {
		userdata = static_cast<CControlUI*>(pLayout->GetItemAt(i))->GetUserData();
		CtlId = StrToInt(userdata)+ 1 - baseData ;
		if (CtlId > 0) {
			if (id < CtlId) 
				break;
			else 
				order++;
		}
		else {
			order++;
		}
	}
	order *= 2;
	return order;
}

void CMyMenuWnd::AddCtl(CameraInfo cameraInfo,CDuiString layoutName, int BaseData)
{
	CVerticalLayoutUI *pLayout;
	CDuiString  userdata;

	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	userdata.Format(_T("%d"), cameraInfo.id + BaseData);
	new CMenuItemUI(pLayout, cameraInfo.Name, userdata, InsertAt(cameraInfo.id, pLayout, BaseData));
}

void CMyMenuWnd::DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData)
{
	CVerticalLayoutUI *pLayout;
	CMenuItemUI *pMenuItem = NULL, *pPrevItem, *pNextItem;
	CDuiString  userdata;
	int Count;
	int id, order;
	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	Count = pLayout->GetCount();
	for (int i = 0; i < Count; i += 2) {
		pMenuItem = (CMenuItemUI*)pLayout->GetItemAt(i);
		id = StrToInt(pMenuItem->GetUserData()) - BaseData;
		if (id == cameraInfo.id) {
			order = pLayout->GetItemIndex(pMenuItem);
			pLayout->RemoveAt(order);
			pLayout->RemoveAt(order);//分割线
			break;
		}
	}
}

void CMyMenuWnd::CameraInfoInit(CameraInfo cameraInfo)
{
	int i= cameraInfo.id;
	camera[i].id = cameraInfo.id;
	camera[i].pTitle->SetText(cameraInfo.Name);
	camera[i].pShipname->SetText(cameraInfo.Name);
	camera[i].pSwitch->SetValue(cameraInfo.IsActivate);
	camera[i].pVolum->SetValue(cameraInfo.Volumn);
	camera[i].pSaveVideo->SetValue(cameraInfo.IsVideoRecordEnabled);
	camera[i].pAutoWatch->SetValue(cameraInfo.IsAutoWatchEnabled);
}

CameraInfo CMyMenuWnd::GetCameraSetInfo(int id)
{
	CameraInfo setInfo;
	setInfo.Name = camera[id].pShipname->GetText();
	setInfo.IsActivate = camera[id].pSwitch->GetValue();
	setInfo.Volumn = camera[id].pVolum->GetValue();
	setInfo.IsVideoRecordEnabled = camera[id].pSaveVideo->GetValue();
	setInfo.IsAutoWatchEnabled = camera[id].pAutoWatch->GetValue();
	return setInfo;
}

void CMyMenuWnd::SetWatchTime(DWORD beginTime,DWORD endTime)
{
	if (beginTime < 1440 && endTime < 1440) {
		CTimeButtonUI *pHour1, *pHour2, *pMinute1, *pMinute2;
		int tHour1, tHour2, tMinute1, tMinute2;
		tHour1 = beginTime / 60;
		tHour2 = endTime / 60;
		tMinute1 = beginTime%60;
		tMinute2 = endTime%60;

		pAwTime[0]->SetValue(tHour1);
		pAwTime[1]->SetValue(tMinute1);
		pAwTime[2]->SetValue(tHour2);
		pAwTime[3]->SetValue(tMinute2);
	}
}

void CMyMenuWnd::GetWatchTime(DWORD* pBegining, DWORD* pEnd)
{	

	*pBegining = pAwTime[0]->GetValue() + pAwTime[1]->GetValue();
	*pEnd = pAwTime[2]->GetValue()+ pAwTime[3]->GetValue();
}

bool CMyMenuWnd::CameraSetIsChange()
{
	CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
	if (pPort) {

	Print("newname id:%d  oldid:%d", camera[pPort->m_Id - 1].pShipname->GetTag(), pPort->GetNameId());
		if (camera[pPort->m_Id - 1].pShipname->GetTag() != pPort->GetNameId())
			return true;
		if (camera[pPort->m_Id - 1].pSwitch->GetValue() != pPort->m_DevConfig.IsCameraOn)
			return true;
		if (camera[pPort->m_Id - 1].pVolum->GetValue() != pPort->m_DevConfig.Volumn)
			return true;
		if (camera[pPort->m_Id - 1].pSaveVideo->GetValue() != pPort->m_DevConfig.IsRecordEnabled)
			return true;
		if (camera[pPort->m_Id - 1].pAutoWatch->GetValue() != pPort->m_DevConfig.IsAutoWatchEnabled)
			return true;
		
		return false;
	}
}

bool CMyMenuWnd::ShipNameIsChange()
{
	CDuiString text;
	std::string oldname = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.boat_name;
	text.Format(_T("%S"), oldname.c_str());
	if (_tcscmp(pShipName->GetText(), text) != 0) {
		return true;
	}
	return false;
}

bool CMyMenuWnd::SysSetIsChange()
{
	pSysLight->GetValue();
	if (pSysLight->GetValue() != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness)
		return true;
	if (pSysVolum->GetValue() != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn)
		return true;
	return false;
}

bool CMyMenuWnd::AwTimeIsChange()
{
	DWORD begine=0, end=0;
	GetWatchTime(&begine, &end);
	if (begine != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_begining)
		return true;
	if (end != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_end)
		return true;
	return false;
}

bool CMyMenuWnd::AlarmVoiceIsChange()
{
	if (pAlmVicSwitch->GetValue() != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_onoff)
		return true;
	if (mAlarmVoiceSel != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_id)
		return true;
	return false;
}

bool CMyMenuWnd::AlarmLightIsChange()
{
	if (pAlarmLight->GetValue() != ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff)
		return true;
	return false;
}



LRESULT CMyMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam)
	{
	case VK_BACK:
		if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ListLabelElementUI")) == 0) {
			WindowImplBase::OnKeyDown(uMsg, wParam, lParam, bHandled);
		}
		//if (m_pm.FindControl(_T("button_home")) == pHomeWatch) {
		//	m_pm.FindControl(_T("homewatch"))->SetFocus();
		//	m_pm.FindControl(_T("layout_home_watch"))->SetVisible(true);
		//}
		else if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ShipNameItemUI")) == 0) {
			ExpandCameraName();
		}
		else {
			if(FocusedItem[1]){
				KeyDown_VK_BACK();
				UpdataItemColor();
			}
		}
		break;
	default:
		WindowImplBase::OnKeyDown(uMsg, wParam, lParam, bHandled);
		break;
	}
	return LRESULT();
}

void CMyMenuWnd::KeyDown_VK_BACK()
{
	int inx;
	int nPort;
	CMyEditUI *pCameraName;
	inx = pLayout_third->GetCurSel();
Print("Third Menu Sel :%d", inx);
	switch (inx) {
	//---------------船舶名称----------------------
	case 6:
		//船名正在编辑
		if (pKeyBoard->IsVisible()) {
			pKeyBoard->SetVisible(false);
			m_pm.FindControl(_T("prompt"))->SetVisible(false); //提示信息
			pShipName->SetFocus();
		}
		else {
			if (ShipNameIsChange()) {
				if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
					CDuiString& boat_name  =  pShipName->GetText();

					CString name = boat_name.GetData();

					char tmp[40];

					WideCharToMultiByte(CP_ACP, 0,  name, -1,  tmp,  40, NULL, false);

					std::string& strName = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.boat_name;

					strName = tmp;

					((CColdEyeApp*)AfxGetApp())->StoreBoatName();
				}
				else {
					std::string& strName = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.boat_name;
					CString name;
					name.Format(_T("%S"), strName.c_str());
					pShipName->SetText(name);
				}
			}
			BackTOMenuItem();
		}

		break;
	//---------------摄像头设置----------------------
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		nPort = inx - 6;
		pCameraName = (CMyEditUI*) static_cast<CContainerUI*>(pLayout_third->GetItemAt(inx))->FindSubControl(_T("edit_cameraname"));
		// 摄像头名称正在编辑
		if (pCameraName->GetStatus()) {
			ExpandCameraName();
			pCameraName->SetFocus();
		}
		else {
			if (CameraSetIsChange()) {
				if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
					//保存设置
					CPort* pPort  = (CPort*)FocusedItem[1]->GetTag();
					if (pPort) {

						DeviceConfig config;
						Print("Set name id is:%d", camera[nPort - 1].pShipname->GetTag());
						if(camera[nPort - 1].pShipname->GetTag())
						config.NameId  = camera[nPort-1].pShipname->GetTag();    //Tag :1~18
						config.IsCameraOn  = camera[nPort-1].pSwitch->GetValue();
						config.Volumn      = camera[nPort-1].pVolum->GetValue();
						config.IsRecordEnabled = camera[nPort-1].pSaveVideo->GetValue();
						config.IsAutoWatchEnabled = camera[nPort-1].pAutoWatch->GetValue();

						CTime time  = CTime::GetCurrentTime();
						CDBLogger* pLogger  = CDBLogger::GetInstance();
						if (pPort->m_DevConfig.IsCameraOn != config.IsCameraOn) {
							pLogger->LogCameraOnOff(time, pPort);
						}
						if (pPort->m_DevConfig.IsAutoWatchEnabled != config.IsAutoWatchEnabled) {
							pLogger->LogCameraAWOnOff(time, pPort);
						}

						::SendMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_CHANGE, (WPARAM)pPort, (LPARAM)&config);
					}
				}
				else {
					CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
					if (pPort) {
						FillPortConfig(pPort);
					}
				}
			}
		}
		BackTOMenuItem();
		break;
	//----------------系统设置---------------------
	case 13:
		if (SysSetIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				//保存设置
				((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness = pSysLight->GetValue();
				((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn = pSysVolum->GetValue();
				((CColdEyeApp*)AfxGetApp())->StoreSystemConfig();
			}
			else {

				pSysLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness);
				pSysVolum->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn);
			}
		}
		BackTOMenuItem();
		break;
	//----------------看船时间---------------------
	case 14:
		Print("Aw time change");
		if (AwTimeIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				//保存看船时间
				DWORD aw_begining, aw_end;
				GetWatchTime(&aw_begining, &aw_end);
				Print("Get watch time:%02d:%02d--%02d:%02d", aw_begining/60, aw_begining%60, aw_end/60, aw_end%60);

				if (((CColdEyeApp*)AfxGetApp())->SetAwTime(aw_begining, aw_end)) {
					MSG msg;
					msg.message = USER_MSG_CAMERA_CONFIG_AWTIME;
					CMsgSquare::GetInstance()->Broadcast(msg);
				}
			}
			else {
				DWORD aw_begining, aw_end;
				aw_begining = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_begining;
				aw_end = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.watch_time_end;
				SetWatchTime(aw_begining,aw_end);
			}
		}
		BackTOMenuItem();
		break;
	//--------------报警音----------------------
	case 15:
		if (AlarmVoiceIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				bool state = pAlmVicSwitch->GetValue() > 0;
				if (state) {
					((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_id = mAlarmVoiceSel;
				}
				((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_onoff = state;

				((CColdEyeApp*)AfxGetApp())->StoreAlarmSoundConfig();
			}
			else {
				bool state = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_onoff ;
				pAlmVicSwitch->SetValue(state);
				ShowAlarmVoiceList(state);
				mAlarmVoiceSel = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_id;
				if (mAlarmVoiceSel) {
					if (pVoice1) {
						pDefaultVoice->SetVoiceSel(false);
						pVoice1->SetVoiceSel(true);
					}
					else {
						pDefaultVoice->SetVoiceSel(true);
					}
				}
				else {
					pDefaultVoice->SetVoiceSel(true);
					pVoice1->SetVoiceSel(false);
				}
			}
		}
		BackTOMenuItem();
		break;
	//-------------报警灯---------------------
	case 16:
		if (AlarmLightIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff = pAlarmLight->GetValue();

				((CColdEyeApp*)AfxGetApp())->StoreAlarmLightConfig();
			}
			else {
				pAlarmLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff);
			}
		}
		BackTOMenuItem();
		break; 
	//----------------自动看船开关记录---------------

	case 17:
		AwPage(1);
		pAwOnOffRecordList->UnSelectAllItems();
		if (FocusedItem[1]) {
			BackTOMenuItem();
		}
		break;
	default:
		if (FocusedItem[1]) {
			BackTOMenuItem();
		}
		m_pm.Invalidate();
		break;
	}
}



CMenuItemUI* CMyMenuWnd::AddMenuItem(CPort* pPort, CDuiString layoutName, int baseData)
{
	CVerticalLayoutUI * pLayout;
	CMenuItemUI* pMenuItem;
	CDuiString   userData;
	CPort*       tagPort = NULL;

	int itemCnt  = 0;

	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	itemCnt  = pLayout->GetCount();

	// 判断是否有item已经对应该port
	for (int i = 0; i < itemCnt; i += 2) {
		pMenuItem  =  (CMenuItemUI*)pLayout->GetItemAt(i);
		tagPort  = (CPort*)pMenuItem->GetTag();
		if (tagPort == pPort) {
			return pMenuItem;
		}
	}
	
	userData.Format(_T("%d"), pPort->GetId()-1 + baseData);
	pMenuItem = new CMenuItemUI(pLayout, pPort->GetName(), userData, InsertAt(pPort->GetId()-1, pLayout, baseData));
	return pMenuItem;
}

void CMyMenuWnd::AddAlarmMenuItem(CPort* pPort)
{
	Print("port id :%d , %d", pPort->GetId(),pPort->m_virginNumber);
	pAlarmItem[pPort->GetId()-1]  = AddMenuItem(pPort, _T("layout_submenu_alarm"), ALARM_VIDEO);
	pAlarmItem[pPort->GetId() - 1]->SetMark(_T("true"));
	pAlarmItem[pPort->GetId() - 1]->SetTag((UINT_PTR)pPort);
	SetAllVirginNum();
}

void CMyMenuWnd::AddVideoObtainMenuItem(CPort* pPort)
{
	CMenuItemUI* pItem = AddMenuItem(pPort, _T("layout_submenu_videoget"),  VIDEO_OBTAIN);
	pItem->SetTag((UINT_PTR)pPort);
}

void CMyMenuWnd::AddPortConfigMenuItem(CPort* pPort)
{
	CMenuItemUI* pItem  = AddMenuItem(pPort, _T("layout_submenu_setting"), CAMERA_SET);
	pItem->SetTag( (UINT_PTR)pPort);
}



void CMyMenuWnd::DeleteMenuItem(CPort* pPort, CDuiString layoutname, int BaseData)
{
	CVerticalLayoutUI* pLayout;

	CMenuItemUI* pMenuItem ;
	CMenuItemUI* pPrevItem;
	CMenuItemUI* pNextItem;

	CPort* tagPort;
	int itemCnt   = 0;

	pLayout  = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutname));
	itemCnt  = pLayout->GetCount();

	for (int i = 0; i < itemCnt; i += 2) {
		pMenuItem  = (CMenuItemUI*)pLayout->GetItemAt(i);

		tagPort  = (CPort*)pMenuItem->GetTag();

		if (tagPort == pPort) {
			Print("Delete item %d port", pPort->m_Id);
			int order  = pLayout->GetItemIndex(pMenuItem);
			pLayout->RemoveAt(order);
			pLayout->RemoveAt(order);
			return ;
		}

		
	}

	Print("Delete item nothing");
}

void CMyMenuWnd::DeleteAlarmMenuItem(CPort* pPort)
{
	DeleteMenuItem(pPort, _T("layout_submenu_alarm"), ALARM_VIDEO);
}


void CMyMenuWnd::DeleteVideoObtainMenuItem(CPort* pPort) {
	DeleteMenuItem(pPort, _T("layout_submenu_videoget"), VIDEO_OBTAIN);
}




void CMyMenuWnd::DeletePortConfigMenuItem(CPort* pPort)
{
	DeleteMenuItem(pPort, _T("layout_submenu_setting"), CAMERA_SET);
}

void CMyMenuWnd::FillPortConfig(CPort* pPort)
{
	int inx = pPort->GetId() - 1;

	camera[inx].pTitle->SetText(pPort->GetName());
	camera[inx].pShipname->SetText(pPort->GetName());
	camera[inx].pShipname->SetTag(pPort->GetNameId());
	camera[inx].pSwitch->SetValue(pPort->m_DevConfig.IsCameraOn);
	camera[inx].pVolum->SetValue(pPort->m_DevConfig.Volumn);
	camera[inx].pSaveVideo->SetValue(pPort->m_DevConfig.IsRecordEnabled);
	camera[inx].pAutoWatch->SetValue(pPort->m_DevConfig.IsAutoWatchEnabled);
}

void CMyMenuWnd::InitAlarmVoice()
{
	bool state;
	mAlarmVoiceSel = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_id;
	state = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_onoff;

	if (CRecordAlarmSound::GetInstance()->ScanVoice()) {
		AddAlarmVoice();
	}

	//选中默认  0:默认，1录制
	if (mAlarmVoiceSel == 0){
		pDefaultVoice->SetVoiceSel(true);
	}
	else {
		if(pVoice1)
			pVoice1->SetVoiceSel(true);
		else 
			pDefaultVoice->SetVoiceSel(true);
	}
	pAlmVicSwitch->SetValue(state);
	ShowAlarmVoiceList(state);
}

void CMyMenuWnd::BackTOMenuItem()
{
	FocusedItem[1]->SetFocus();
	FocusedItem[1] = NULL;
	focusLevel--;
	m_pm.Invalidate();
}
