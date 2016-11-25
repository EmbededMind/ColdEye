#include "stdafx.h"
//
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

#include "Control\PopupMenuUI.h"
#include "Control\TimeButtonUI.h"

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
	//-------------------------菜单控件关联-----------------------------------------
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

	//-------------------------看船时间控件关联---------------------------------
	CTimeButtonUI *pTimeBt[4]; 
	pTimeBt[0] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_hour")));
	pTimeBt[1] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_minute")));
	pTimeBt[2] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_hour")));
	pTimeBt[3] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_minute")));
	pTimeBt[0]->SetItemRelation(NULL, pTimeBt[1]);
	pTimeBt[1]->SetItemRelation(pTimeBt[0], pTimeBt[2]);
	pTimeBt[2]->SetItemRelation(pTimeBt[1], pTimeBt[3]);
	pTimeBt[3]->SetItemRelation(pTimeBt[2], NULL);

	//-------------------------摄像机设置界面添加-------------------------------
	CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	CDialogBuilder Camera1builder, Camera2builder, Camera3builder, Camera4builder, Camera5builder, Camera6builder;
	CVerticalLayoutUI *CamareChildLayout;
	CamareChildLayout = (CVerticalLayoutUI*)Camera1builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 7);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头1设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera2builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 8);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头2设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera3builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 9);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头3设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera4builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 10);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头4设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera5builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 11);
	GetCameraItem(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头5设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera6builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 12);
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

void CMyMenuWnd::Notify(TNotifyUI & msg)
{
	
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
		case USER_MSG_LOGIN:
			Print("Menu case login msg");

			AddPortConfigMenuItem( (CPort*)lParam);
			FillPortConfig( (CPort*)lParam);
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
		case WM_KEYDOWN: {
			if (GetKeyState(VK_CONTROL) && !(wParam & 0x20000000)) {
					if (wParam == 'U'){

					}
				}
			}
			break;

		case USER_MSG_CAMERA_CONFIG_AWTIME:
			DWORD aw_begining, aw_end;
			GetWatchTime(&aw_begining, &aw_end);
			if (((CColdEyeApp*)AfxGetApp())->SetAwTime(aw_begining, aw_end)) {
				MSG msg;
				msg.message = USER_MSG_CAMERA_CONFIG_AWTIME;
				CMsgSquare::GetInstance()->Broadcast(msg);
			}
			break;
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

void CMyMenuWnd::Relationship(CVerticalLayoutUI * pLayout, CMenuItemUI * pMenuItem)
{
	CMenuItemUI *pPreItem, *pNextItem;
	int iInx = pLayout->GetItemIndex(pMenuItem);
	pPreItem = (CMenuItemUI*)pLayout->GetItemAt(iInx - 2);
	pNextItem = (CMenuItemUI*)pLayout->GetItemAt(iInx + 2);
	pMenuItem->SetItemRelation(pPreItem, pNextItem);
	if (pPreItem)
		pPreItem->SetNextItem(pMenuItem);
	if (pNextItem)
		pNextItem->SetPrevItem(pMenuItem);
}

void CMyMenuWnd::AddCtl(CameraInfo cameraInfo,CDuiString layoutName, int BaseData)
{
	CVerticalLayoutUI *pLayout;
	CMenuItemUI *pMenuItem;
	CDuiString  userdata;

	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	userdata.Format(_T("%d"), cameraInfo.id + BaseData);
	pMenuItem = new CMenuItemUI(pLayout, cameraInfo.Name, userdata, InsertAt(cameraInfo.id, pLayout, BaseData));
	Relationship(pLayout, pMenuItem);
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
	if (pMenuItem) {
		pPrevItem = (CMenuItemUI*)pMenuItem->GetPrevItem();
		pNextItem = (CMenuItemUI*)pMenuItem->GetNextItem();
		if (pPrevItem)
			pPrevItem->SetNextItem(pNextItem);
		if (pNextItem)
			pNextItem->SetPrevItem(pPrevItem);
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

		pHour1 = (CTimeButtonUI*)m_pm.FindControl(_T("time1_hour"));
		pHour2 = (CTimeButtonUI*)m_pm.FindControl(_T("time2_hour"));
		pMinute1 = (CTimeButtonUI*)m_pm.FindControl(_T("time1_minute"));
		pMinute2 = (CTimeButtonUI*)m_pm.FindControl(_T("time2_minute"));
		pHour1->SetValue(tHour1);
		pHour2->SetValue(tHour2);
		pMinute1->SetValue(tMinute1);
		pMinute2->SetValue(tMinute2);
	}
}

void CMyMenuWnd::GetWatchTime(DWORD* pBegining, DWORD* pEnd)
{	
	// 返回看船时间段 ，不确定该如何返回给露露，暂时定的两个变量
	CTimeButtonUI *pHour1, *pHour2, *pMinute1, *pMinute2;
	pHour1 = (CTimeButtonUI*)m_pm.FindControl(_T("time1_hour"));
	pHour2 = (CTimeButtonUI*)m_pm.FindControl(_T("time2_hour"));
	pMinute1 = (CTimeButtonUI*)m_pm.FindControl(_T("time1_minute"));
	pMinute2 = (CTimeButtonUI*)m_pm.FindControl(_T("time2_minute"));

	Print("hour:%d minute:%d   hour:%d minute:%d", pHour1->GetValue(), pMinute1->GetValue(),
	                                               pHour2->GetValue(), pMinute2->GetValue());

	*pBegining = pHour1->GetValue() + pMinute1->GetValue();
	*pEnd = pHour2->GetValue()+ pMinute2->GetValue();
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

CMenuItemUI* CMyMenuWnd::AddMenuItem(CPort* pPort, CDuiString layoutName, int baseData)
{
	CVerticalLayoutUI * pLayout;
	CMenuItemUI* pMenuItem;
	CDuiString   userData;

	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(layoutName));
	userData.Format(_T("%d"), pPort->GetId()-1 + baseData);
	pMenuItem = new CMenuItemUI(pLayout, pPort->GetName(), userData, InsertAt(pPort->GetId()-1, pLayout, baseData));

	Relationship(pLayout, pMenuItem);

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