#include "stdafx.h"
//
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"
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
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头1设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera2builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 8);
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头2设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera3builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 9);
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头3设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera4builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 10);
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头4设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera5builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 11);
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头5设置"));
	CamareChildLayout = (CVerticalLayoutUI*)Camera6builder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pTabLayout->AddAt(CamareChildLayout, 12);
	CameraSetItemInit(CamareChildLayout);
	static_cast<CVerticalLayoutUI*>(CamareChildLayout->GetItemAt(0))->GetItemAt(0)->SetText(_T("摄像头6设置"));

	//添加摄像头
	CameraInfo cameraInfo;
	cameraInfo.Name = _T("摄像头1");
	cameraInfo.Volumn = 5;
	cameraInfo.IsActivate = true;
	cameraInfo.id = 2;
	AddCamear(cameraInfo);

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
		switch (wParam) {
		case SHIPNAME_LIMIT:
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_camera_switch.xml"), NULL, NULL));
			break;
			
		case CLOSE_CAMERA:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL);
			break;

		case CLOSE_STROAGE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"),_T("关闭储存摄像机视频后，您将无法回"), _T("放视频，是否确定关闭？"));
			break;

		case CLOSE_AUTOWATCH:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"),_T("关闭摄像机自动看船后，该摄像头将"), _T("不会发生报警，是否确定关闭？"));
			break;

		case SAVE_CHANGES:
			break;

		case UPDATE_REQUEST:
			CMsgWnd::ShowMessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL);
			//CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update_request.xml"), NULL, NULL);
			break;
			
		case NO_UPDATE_FILE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_ok.xml"), _T("U盘未发现更改版本的软件更新程"), _T("序！"));
			break;

		case NO_UPDATE_DRIVE:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_ok.xml"), NULL, _T("未检测到U盘，请重试！"));
			break;

		case SOFT_UPDATING:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update.xml"), _T("V2.0.0"), NULL);
			break;

		case SOFT_UPDATE_SUCCESS:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_update_success.xml"), _T("软件版本：V2.0.0"), NULL);
			break;

		case FACTORY_RESET:
			CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("确定恢复出厂设置？"));
			break;

		case RECORD:
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
			break;

		case COPYING:
			break;

		case STOP_COPY:
			break;

		case COPY_SUCCESS:
			break;

		case COPY_FAILURE:
			break;
		}
	}
	return LRESULT();
}

void CMyMenuWnd::CameraSetItemInit(CVerticalLayoutUI * pLayout)
{
	CTabLayoutUI *pTabLayout;
	CContainerUI  *pChildLayout;
	int iInx;
	pTabLayout = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	iInx =pTabLayout->GetItemIndex(pLayout)-7;
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(0));
	cameraset[iInx].pShipname = (CMyEditUI*)pChildLayout->GetItemAt(2);	//船名
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(2));
	cameraset[iInx].pSwitch = (CCameraSwitchUI*)pChildLayout->GetItemAt(1);//摄像头开关
	cameraset[iInx].pVolum = (CMySliderUI*)pChildLayout->GetItemAt(4);//音量
	cameraset[iInx].pVideoSave = (CMyLabelUI*)pChildLayout->GetItemAt(6);//视频存储设置
	cameraset[iInx].pAutoWatch = (CMyLabelUI*)pChildLayout->GetItemAt(8);//自动看船设置
}

//void CMyMenuWnd::AddCamear(UINT8 iInx)
//{ 
//	//--二级菜单--
//	//报警视频
//	CContainerUI* pLayout;
//	CMenuItemUI *pMenuItem;
//	CDuiString text, userdata, name;
//	UINT8 Count,order=0;
//	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("layout_submenu_alarm")));
//	userdata.Format(_T("%d"), iInx - 1);
//	Count = pLayout->GetCount() / 2;
//	text.Format(_T("摄像头%d"), iInx);
//	for (int i = 0; i < Count; i++) {
//		userdata = static_cast<CControlUI*>(pLayout->GetItemAt(i))->GetUserData();
//		if (iInx < StrToInt(userdata)+1) {
//			break;
//		}
//		else {
//			order++;
//		}
//	}
//	order *= 2;
//	pMenuItem = new CMenuItemUI(pLayout, text, order);
//	pMenuItem->SetUserData(userdata);
//	pMenuItem->SetPrevItem(pLayout->GetItemAt(order - 2));
//	pMenuItem->SetNextItem(pLayout->GetItemAt(order + 2));
//}

void CMyMenuWnd::AddCamear(CameraInfo cameraInfo)
{
	CVerticalLayoutUI *pLayout;
	CMenuItemUI *pMenuItem;
	CDuiString  userdata, name;
	int Count;











	//报警视频二级菜单
	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("layout_submenu_alarm")));
	Count = pLayout->GetCount()/2+1;
	pMenuItem = new CMenuItemUI(pLayout, cameraInfo.Name);
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
	
	//摄像头设置控件
	pLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("layout_submenu_setting")));
	pMenuItem = new CMenuItemUI(pLayout, cameraInfo.Name, (Count * 2));
	userdata.Format(_T("%d"), Count + 6);	//设置控件数据
	name.Format(_T("cameraset%d"), Count);
	pMenuItem->SetName(name);					//设置控件名称
	pMenuItem->SetUserData(userdata);
	static_cast<CMenuItemUI*>(pLayout->GetItemAt((Count - 1) * 2))->SetNextItem((CControlUI*)pMenuItem); //主机名称
	pMenuItem->SetPrevItem((CControlUI*)(pLayout->GetItemAt((Count - 1) * 2))); 
	pMenuItem->SetNextItem((CControlUI*)pLayout->GetItemAt((Count+1)*2)); 
	static_cast<CMenuItemUI*>(pLayout->GetItemAt((Count + 1) * 2))->SetPrevItem((CControlUI*)pMenuItem); //系统设置
}

void CMyMenuWnd::SetCameraInfo()
{
}

