#include "stdafx.h"
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

#include "Control\PopupMenuUI.h"

#include "Pattern\MsgSquare.h"

#include "Database\DBShadow.h"
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
	pKeyBoard = static_cast<CContainerUI*>(m_pm.FindControl(_T("keyboard")));
	pSysLight = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_light")));
	pSysVolum = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_voice")));
	pAlmVicSwitch = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmvoice_switch")));
	pDefaultVoice = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("alarmvoice_default")));
	pVoice1 = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("voice1")));
	pAlarmLight = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmlight_switch")));
	pAwOnOffRecordList = static_cast<CListUI*>(m_pm.FindControl(_T("watch_record")));
	pPage = static_cast<CLabelUI*>(m_pm.FindControl(_T("page")));
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
				
			}
			else if (pNextFocusLayout->GetCount() > 0) {
				pLayout_third->SetVisible(true);
				pNextFocusLayout->GetItemAt(0)->SetFocus();
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
			pLayout_third->SelectItem(0);
			pLayout_third->SetVisible(false);
			break;
			//-----------------------------------------------
		case VK_RIGHT:
			FocusedItem[1] = pItem;
			ThirdMenuSetFocus(userdata);
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
			pDefaultVoice->SetVoiceSel(true);
			if(pVoice1)
				pVoice1->SetVoiceSel(false);
		}
		else {
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
	switch (msg.wParam) {
	case VK_BACK:
		if (FocusedItem[1]) {
			CVideoListUI* list = (CVideoListUI*)m_pm.GetFocus()->GetParent()->GetParent();
			list->UnSelectAllItems();
			FocusedItem[1]->SetFocus();
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
	}
	else {
		pChildLayout1->SetVisible(false);
		rect = pChildLayout2->GetPos();
		size.cx = 70;
		size.cy = rect.top - 204;
		pChildLayout2->SetFixedXY(size);
		CMyEditUI *pItem = (CMyEditUI*)(static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(0))->GetItemAt(2));
		pItem->SetStatus(false);
	}
}

void CMyMenuWnd::ShowAlarmVoiceList(bool Switch)
{
	int inx;
	inx = pLayout_third->GetCurSel();
	CContainerUI* pContain = (CContainerUI*)pLayout_third->GetItemAt(inx);
	pContain = (CContainerUI*)pContain->GetItemAt(1);
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
		if (!mPlayerWall) {
			mPlayerWall = new CPlayerWallWnd(_T("playerwall.xml"));
			mPlayerWall->Create(NULL, _T("PlayerWallWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, { 0,0,0,0 });
			mPlayerWall->ShowWindow(true);
			mPlayerWall->CenterWindow();
			::SendMessage(mPlayerWall->GetHWND(), USER_MSG_PLAY_START, msg.wParam, msg.lParam);
		}
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
		case USER_MSG_LOGOFF:
			Print("Menu case logoff msg");
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
							AddAlarmMenuItem(pPortMgr->GetPortById(i+1));
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
		mTotalPage = stmt->ValueInt(0);
	}
	if (mTotalPage % 13) {
		mTotalPage = mTotalPage / 13 + 1;
	}
	else {
		mTotalPage = mTotalPage / 13;
	}
	printf("mTotalPage : %d\n", mTotalPage);
	if(mTotalPage>0)
		mPage = 1;
	else mPage = 0;
	sPage.Format(_T("第%d/%d"), mPage, mTotalPage);
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
	if (page <= mTotalPage && page>0) {
		mPage = page;
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
		sPage.Format(_T("第%d/%d"), page, mTotalPage);
		pPage->SetText(sPage);
	}
}

void CMyMenuWnd::AwOnOffRecordNextPage()
{
	if (mPage < mTotalPage) {
		mPage++;
		AwPage(mPage);
	}
}

void CMyMenuWnd::AwOnOffRecordLastPage()
{
	if (mPage > 1) {
		mPage--;
		AwPage(mPage);
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
		pAwTime[1]->SetValue(tHour2);
		pAwTime[2]->SetValue(tMinute1);
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
	return true;
}

bool CMyMenuWnd::ShipNameIsChange()
{
	pShipName->GetText();
	return false;
}

bool CMyMenuWnd::SysSetIsChange()
{
	pSysLight->GetValue();
	pSysVolum->GetValue();
	return false;
}

bool CMyMenuWnd::AwTimeIsChange()
{
	DWORD *begine=NULL, *end=NULL;
	//GetWatchTime(begine, end);
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
		else {
			KeyDown_VK_BACK();
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
					Print("Message ok");
				}
				else {
					Print("Message cancel");
				}
			}
			else if(FocusedItem[1]) {
				FocusedItem[1]->SetFocus();
			}
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
						config.IsCameraOn  = camera[nPort-1].pSwitch->GetValue();
						config.Volumn      = camera[nPort-1].pVolum->GetValue();
						config.IsRecordEnabled = camera[nPort-1].pSaveVideo->GetValue();
						config.IsAutoWatchEnabled = camera[nPort-1].pAutoWatch->GetValue();

						::SendMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_CHANGE, (WPARAM)pPort, (LPARAM)&config);
					}
					//Print("Camera %d ,Vol:%d", nPort, camera[nPort-1].pVolum->GetValue());
					//::SendMessage( ((CColdEyeDlg*)AfxGetMainWnd())->m_hWnd, );
				}
				else {
					//恢复设置
				}
			}
			else if(FocusedItem[1]){
				FocusedItem[1]->SetFocus();
			}
		}

		break;
	//----------------系统设置---------------------
	case 13:
		if (SysSetIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				//保存设置
			}
			else {
				//恢复之前设置
			}
		}
		else if(FocusedItem[1]){
			FocusedItem[1]->SetFocus();
		}
		break;
	//----------------看船时间---------------------
	case 14:
		if (AwTimeIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL, NULL)) {
				//保存看船时间
			}
			else {
				//恢复之前设置的看船时间
			}
		}
		else if(FocusedItem[1]){
			FocusedItem[1]->SetFocus();
		}
		break;
	//----------------自动看船开关记录---------------
	case 17:
		AwPage(1);
		pAwOnOffRecordList->UnSelectAllItems();
		if (FocusedItem[1]) {
			FocusedItem[1]->SetFocus();
		}
		break;
	default:
		if (FocusedItem[1]) {
			FocusedItem[1]->SetFocus();
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

	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	userData.Format(_T("%d"), pPort->GetId()-1 + baseData);
	pMenuItem = new CMenuItemUI(pLayout, pPort->GetName(), userData, InsertAt(pPort->GetId()-1, pLayout, baseData));
	return pMenuItem;
}

void CMyMenuWnd::AddAlarmMenuItem(CPort* pPort)
{
	CMenuItemUI* pItem  = AddMenuItem(pPort, _T("layout_submenu_alarm"), ALARM_VIDEO);
	pItem->SetTag((UINT_PTR)pPort);
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

void CMyMenuWnd::FillPortConfig(CPort* pPort)
{
	int inx = pPort->GetId() - 1;

	camera[inx].pTitle->SetText(pPort->GetName());
	camera[inx].pShipname->SetText(pPort->GetName());
	camera[inx].pSwitch->SetValue(pPort->m_DevConfig.IsCameraOn);
	camera[inx].pVolum->SetValue(pPort->m_DevConfig.Volumn);
	camera[inx].pSaveVideo->SetValue(pPort->m_DevConfig.IsRecordEnabled);
	camera[inx].pAutoWatch->SetValue(pPort->m_DevConfig.IsAutoWatchEnabled);
}

void CMyMenuWnd::InitAlarmVoice()
{
	int Sel = 1;
	int VoiveNum= 2;
	int AlarmOnOff = 0;
	int VoiceSel = 0;
	char sqlStmt[128];
	sprintf_s(sqlStmt, "SELECT * FROM host_config;");
	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);
	while (stmt->NextRow()) {
		AlarmOnOff = stmt->ValueInt(4);
		VoiceSel = stmt->ValueInt(5);
	}

	for (int i = 0; i < VoiveNum-1; i++) {
		AddAlarmVoice();
	}
	//选中默认
	if (Sel == 1){
		pDefaultVoice->SetVoiceSel(true);
	}
	else {
		if(pVoice1)
		pVoice1->SetVoiceSel(true);
	}
}
