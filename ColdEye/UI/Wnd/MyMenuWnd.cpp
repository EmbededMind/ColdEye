#include "stdafx.h"
//
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

#include "Control\PopupMenuUI.h"

#include "Pattern\MsgSquare.h"

#include "Database\DBShadow.h"


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

}


void CMyMenuWnd::OnFinalMessage(HWND hWnd)
{
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
	//SetItemBkColor(NULL,0xFFEFEFF4, 0xFFFFFFFF);
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
		if (!mPlayer) {
			mPlayer = new CPlayer(_T("Player.xml"));
			mPlayer->Create(NULL, _T("Player"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE, { 0,0,0,0 });
			mPlayer->ShowWindow(true);
			mPlayer->CenterWindow();
			::SendMessage(mPlayer->GetHWND(), USER_MSG_PLAY, msg.wParam, msg.lParam);
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
	if (uMsg == USER_MSG_MESSAGE_BOX) {
		MyMessageBox(uMsg, wParam, lParam, bHandled);
	}

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
	CTabLayoutUI *pTabLayout;
	CContainerUI  *pChildLayout;
	CDuiString name;
	int iInx;
	pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	iInx =pTabLayout->GetItemIndex(pLayout)-7;
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(0));
	camera[iInx].pTitle = (CLabelUI*)pChildLayout->GetItemAt(0);		//标题
	camera[iInx].pShipname = (CMyEditUI*)pChildLayout->GetItemAt(2);	//船名
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(2));
	camera[iInx].pSwitch = (CCameraSwitchUI*)pChildLayout->GetItemAt(1);//摄像头开关
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

void CMyMenuWnd::AddWatchRecord(SwtichRecord info)
{
	CListUI *pList;
	int Count;
	pList = static_cast<CListUI*>(m_pm.FindControl(_T("watch_record")));
	CSwitchRecordListUI *pListEle;
	pListEle = new CSwitchRecordListUI(info.time, info.record_type);
	pList->Add(pListEle);
	pListEle->SetAttribute(_T("style"), _T("watch_record_style"));
	Count = pList->GetCount();
	if (Count % 2)
		pListEle->SetBkColor(0xFFF3F3F3);
	else
		pListEle->SetBkColor(0xFFE5E5E5);
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
	camera[i].pSwitch->SetSwitch(cameraInfo.IsActivate);
	camera[i].pVolum->SetValue(cameraInfo.Volumn);
	camera[i].pSaveVideo->SetValue(cameraInfo.IsVideoRecordEnabled);
	camera[i].pAutoWatch->SetValue(cameraInfo.IsAutoWatchEnabled);
}

CameraInfo CMyMenuWnd::GetCameraSetInfo(int id)
{
	CameraInfo setInfo;
	setInfo.Name = camera[id].pShipname->GetText();
	setInfo.IsActivate = camera[id].pSwitch->GetSwitch();
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
	return false;
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




void CMyMenuWnd::MyMessageBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	int result;
	int inx;
	switch (wParam) {
		case SHIPNAME_LIMIT:
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_camera_switch.xml"), NULL, NULL,NULL,NULL)) {

			}
			break;
		case CLOSE_CAMERA:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL, NULL,NULL);
			break;

		case CLOSE_STROAGE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"),_T("关闭储存摄像机视频后，您将无法回"), _T("放视频，是否确定关闭？"), NULL,NULL);
			break;

		case CLOSE_AUTOWATCH:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"),_T("关闭摄像机自动看船后，该摄像头将"), _T("不会发生报警，是否确定关闭？"), NULL,NULL);
			break;

		case SAVE_CHANGES:
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定更改设置内容？"), NULL,NULL)) {
				;
			}
			else {

			}
			break;

		case UPDATE_REQUEST:
			//CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL);
			//CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update_request.xml"), NULL, NULL);
			break;
			
		case NO_UPDATE_FILE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_ok.xml"), _T("U盘未发现更改版本的软件更新程"), _T("序！"), NULL,NULL);
			break;

		case NO_UPDATE_DRIVE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_ok.xml"), NULL, _T("未检测到U盘，请重试！"), NULL,NULL);
			break;

		case SOFT_UPDATING:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL, NULL,NULL);
			break;

		case SOFT_UPDATE_SUCCESS:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update_success.xml"), _T("软件版本：V2.0.0"), NULL, NULL,NULL);
			break;

		case FACTORY_RESET:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定恢复出厂设置？"), NULL,NULL);
			break;

		case RECORD: 
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_recordingvoice.xml"), NULL, NULL, NULL,NULL);
			result = CMsgWnd::MessageBox(this->GetHWND(), _T("mb_playvoice.xml"), NULL, NULL, NULL,NULL);
			if (MSGID_OK == result) {

			}
			else if(MSGID_CANCEL == result) {

			}
			else {

			}
			break;

		case SAVE_RECORDED:
			break;

		case DELETE_CAMERA:
			break;

		case DISPLAY_MATCH:
			break;

		case CANNOT_COPY:
			break;

		case NO_COPY_DRIVE:
			break;

		case NO_ENOUGH_MEMORY:
			break;

		case CONFIRM_COPY: 
			{
				//CTime tbegin,tend;
				//CDuiString Text;
				//list<CRecordFileInfo*> *pInfo = (list<CRecordFileInfo*>*)lParam;
				//list<CRecordFileInfo*>::iterator iter;
				//for (iter = pInfo->begin(); iter != pInfo->end(); iter++)
				//	Print("beg = %d\n",(*iter)->tBegin);
				//tbegin = CTime(pInfo->front()->tBegin);
				//tend = CTime(pInfo->front()->tEnd);
				//Text = tbegin.Format("%m月%d日   ") + tbegin.Format("%H:%M:%S") + _T("-") + tend.Format("%H:%M:%S");
				//CMsgWnd::MessageBox(this->GetHWND(), _T("mb_copyvideo_request.xml"), Text, NULL,NULL);
			}
			break;

		case COPYING_ALARM: 
		case COPYING_NORMAL:
			{
				if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_copyvideo.xml"), _T("摄像头1"), NULL, lParam,wParam==COPYING_ALARM?1:0)) {
					CMsgWnd::MessageBox(this->GetHWND(), _T("mb_ok.xml"), NULL, _T("复制成功！"), NULL, NULL);
					}
			}
			break;

		case STOP_COPY:
			break;

		case COPY_SUCCESS:
			break;

		case COPY_FAILURE:
			break;
	}
}

LRESULT CMyMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam)
	{
	case VK_BACK:
		KeyDown_VK_BACK();
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
	Print("inx:%d", inx);

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
	default:
		CVideoListUI *list=NULL;
		if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ListLabelElementUI")) == 0) {
			list = (CVideoListUI*)m_pm.GetFocus()->GetParent()->GetParent();
			list->UnSelectAllItems();
		}
		if (FocusedItem[1]) {
			FocusedItem[1]->SetFocus();
		}
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
	camera[inx].pSwitch->SetSwitch(pPort->m_DevConfig.IsCameraOn);
	camera[inx].pVolum->SetValue(pPort->m_DevConfig.Volumn);
	camera[inx].pSaveVideo->SetValue(pPort->m_DevConfig.IsRecordEnabled);
	camera[inx].pAutoWatch->SetValue(pPort->m_DevConfig.IsAutoWatchEnabled);
}