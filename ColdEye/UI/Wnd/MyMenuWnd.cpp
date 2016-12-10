#include "stdafx.h"
#include "Wnd\MyMenuWnd.h"
#include "Wnd\MsgWnd.h"

#include "Control\PopupMenuUI.h"

#include "Pattern\MsgSquare.h"
 
#include "Database\DBShadow.h"
#include "Database\DBLogger.h"

#include "Com\RecordAlarmSound.h"
#include "Com\MCI.h"
#include "Com\Communication.h"

CDuiString  _PortName[24] = {  //��Ӧ����id
	_T("�����1"),
	_T("�����2"),
	_T("�����3"),
	_T("�����4"),
	_T("�����5"),
	_T("�����6"),
	_T("��ͷ�����"),
	_T("���װ������"),
	_T("�����������"),
	_T("�����������"),
	_T("����"),
	_T("������"),
	_T("����̨"),
	_T("���̨"),
	_T("������"),
	_T("��Ա��"),
	_T("������"),
	_T("�������"),
	_T("�޾��װ�"),
	_T("һ��װ�"),
	_T("����װ�"),
	_T("����װ�"),
	_T("�Ĳ�װ�"),
	_T("���װ�")
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
	//AdapTive();
	//m_scale = 1;
	// 1440*1080
	m_scale = 0.9;
	mDPI = 86;
	m_pm.SetDPI(mDPI);

	pLayout_PopMenu = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_popmenu")));
	pLayout_Menuitem = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_menuitem")));
	pLayout_third = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("layout_thirdmenu")));
	pLayout_HomeWatch = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("home")));
	pShipName = static_cast<CMyEditUI*>(m_pm.FindControl(_T("edit_shipname")));
	std::string& tmp = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.boat_name;
	CString text;
	text.Format(_T("%S"), tmp.c_str());
	pShipName->SetText(text);

	pKeyBoard = static_cast<CContainerUI*>(m_pm.FindControl(_T("keyboard")));

	pSysLight = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_light")));
	pSysLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.brightness);
	pSysLight->bindControl = static_cast<CLabelUI*>(m_pm.FindControl(_T("bind_light")));

	pSysVolum = static_cast<CMySliderUI*>(m_pm.FindControl(_T("sysset_voice")));
	pSysVolum->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn);
	pSysVolum->bindControl = static_cast<CLabelUI*>(m_pm.FindControl(_T("bind_voice")));


	pAlmVicSwitch = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmvoice_switch")));
	pDefaultVoice = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("alarmvoice_default")));
	pVoice1 = static_cast<CAlarmVoiceListUI*>(m_pm.FindControl(_T("voice1")));

	pAlarmLight = static_cast<CSwitchUI*>(m_pm.FindControl(_T("alarmlight_switch")));
	pAlarmLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff);

	pAwOnOffRecordList = static_cast<CListUI*>(m_pm.FindControl(_T("watch_record")));
	pPage = static_cast<CLabelUI*>(m_pm.FindControl(_T("page")));

	pHome = static_cast<CButtonUI*>(m_pm.FindControl(_T("homewatch")));
	pHomeWatchOpen = static_cast<CButtonUI*>(m_pm.FindControl(_T("button_home")));
	if (pHomeWatchOpen) pHomeWatchOpen->OnNotify += MakeDelegate(this, &CMyMenuWnd::OnHomeWatch);
	//-------------------------����ʱ��ؼ�����---------------------------------
	pAwTime[0] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_hour")));
	pAwTime[1] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time1_minute")));
	pAwTime[2] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_hour")));
	pAwTime[3] = static_cast<CTimeButtonUI*>(m_pm.FindControl(_T("time2_minute")));

	pAwTime[0]->SetItemRelation(NULL, pAwTime[1]);
	pAwTime[1]->SetItemRelation(pAwTime[0], pAwTime[2]);
	pAwTime[2]->SetItemRelation(pAwTime[1], pAwTime[3]);
	pAwTime[3]->SetItemRelation(pAwTime[2], NULL);

	//-------------------------��������ý������-------------------------------
	for (int i = 7; i < 13; i++)
		CreataCameraLayout(i);

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

	CPort* pPort = new CPort();
	pPort->SetId(1);
	pPort->SetNameId(1);

	AddPortConfigMenuItem(pPort);
}


void CMyMenuWnd::CreataCameraLayout(int inx)
{
	CDialogBuilder Camerabuilder;
	CVerticalLayoutUI *CamareChildLayout;
	CamareChildLayout = (CVerticalLayoutUI*)Camerabuilder.Create(_T("cameraset.xml"), NULL, this, &m_pm, NULL);
	pLayout_third->AddAt(CamareChildLayout, inx);
	GetCameraItem(CamareChildLayout);
	BingingCameraSlider(CamareChildLayout);
}


void CMyMenuWnd::BingingCameraSlider(CVerticalLayoutUI* pLayout)
{
	CControlUI* pSliderBind = (CControlUI*)static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(2))->FindSubControl(_T("bind_cameraset"));
	CMySliderUI* pSlider = (CMySliderUI*)static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(2))->FindSubControl(_T("camera_set_volume"));
	pSlider->bindControl = (CLabelUI*)pSliderBind;
}

void CMyMenuWnd::AdapTive()
{
	m_DispSize = 1600;
	CDPI* pDpi = m_pm.GetDPIObj();
	mDPI = pDpi->GetDPI();
	m_scale=1;
	//pDpi->SetDPIAwareness(PROCESS_DPI_UNAWARE);
	// 4:3
	float iWidth = GetSystemMetrics(SM_CXSCREEN);
	float iHeight = GetSystemMetrics(SM_CYSCREEN);
	//4:3
	if (13 == (int)((iWidth / iHeight) * 10)) {
		m_scale = ((int)((iWidth / m_DispSize) * 100)) / 100.0;
		mDPI = m_scale*pDpi->GetDPI();
		Print("mdpi:%d", mDPI);
		m_pm.SetDPI(mDPI);
	}
	Print("Scale:%d,DPI:%d", pDpi->GetScale());
}


bool CMyMenuWnd::OnHomeWatch(void * param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	//Print("home message :%S", pMsg->sType);
	if (pMsg->sType == DUI_MSGTYPE_CLICK) {
		pLayout_HomeWatch->SelectItem(1);
		m_pm.FindControl(_T("button_home"))->SetFocus();
	}
	return true;
}

void CMyMenuWnd::UpdataItemColor()
{
	if (focusLevel == 0) {
		//����
		pLayout_PopMenu->SetBkColor(LAYOUT_POP_FOCUSED);
		pLayout_Menuitem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);

		//һ���˵�
		CButtonUI* pItem;
		for (int i = 0; i < pLayout_PopMenu->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout_PopMenu->GetItemAt(i);
			if(pItem != FocusedItem[0])
				pItem->SetBkColor(LAYOUT_POP_FOCUSED);
		}
		FocusedItem[0]->SetTextColor(0xFF666666);
		FocusedItem[0]->SetFocusedTextColor(0xFFFFFFFF);
		//�����˵�
		CVerticalLayoutUI* pLayout = (CVerticalLayoutUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel());
		for (int i = 0; i < pLayout->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_MENUITEM_NOFOCUS);
		}
	}
	else if (focusLevel==1) {
		//����
		pLayout_PopMenu->SetBkColor(LAYOUT_POP_NOFOCUS);
		pLayout_Menuitem->SetBkColor(LAYOUT_MENUITEM_FOCUSED);
		pLayout_third->SetBkColor(LAYOUT_THIRD_NOFOUCS);
		//һ���˵�
		CButtonUI* pItem;
		for (int i = 0; i < pLayout_PopMenu->GetCount(); i += 2) {
			pItem = (CButtonUI*)pLayout_PopMenu->GetItemAt(i);
			pItem->SetBkColor(LAYOUT_POP_NOFOCUS);
		}
		FocusedItem[0]->SetBkColor(POPMENU_SEL_NOFOCUS);
		FocusedItem[0]->SetTextColor(0xFFFFFFFF);
		//�����˵�
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
		//����
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
			pParentLayout->FindSubControl(_T("camera_switch"))->SetFocus();
		}
		else if (sName == _T("sysset_voice")) {
			//m_pm.FindControl(_T("sysset_light"))->SetFocus();
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
			pParentLayout->FindSubControl(_T("camera_set_video_save"))->SetFocus();
		}
		break;
		//-------------------------------------------------------
<<<<<<< HEAD
	//case VK_BACK:
	//	
	//	if (sName == _T("camera_set_volume")) {
	//		CameraInfo info;
	//		info.Volumn  = 1;

	//		CMenuItemUI* pMenuItem  = (CMenuItemUI*)pLayout_third->GetItemAt(7);
	//		::SendMessage( ((CColdEyeDlg*)AfxGetMainWnd())->m_hWnd,  USER_MSG_CAMERA_CONFIG_CHANGE, DEV_CONFIG_MASK_VOL, (LPARAM)NULL);			
	//	}
	//	break;
=======
>>>>>>> bb77dead5ced5c08f54c6bbbe47ec759b166d6c2
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
			CVerticalLayoutUI *pBottomLayout = (CVerticalLayoutUI*)pVerLayout->GetItemAt(2); //����²���
			pBottomLayout->GetItemAt(1)->SetFocus();
		}
		break;
	//-----------------------------------------------
	case VK_RETURN:
		pItem->SetStatus(true);
		if (sName == _T("edit_shipname")) {
			pKeyBoard->SetVisible(true);
			m_pm.FindControl(_T("prompt"))->SetVisible(true); //��ʾ��Ϣ
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
	int ItemIndex = pLayout->GetItemIndex(pItem); //�ؼ�������
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
			pNextFocusLayout = (CContainerUI*)pLayout_Menuitem->GetItemAt(pLayout_Menuitem->GetCurSel()); //��һ������Ĳ���
			if (userdata == _T("4")) {
				pHomeWatchOpen->SetFocus();
				pHome->SetBkColor(0xFF47688F);
				pHome->SetTextColor(0xFFFFFFFF);
			}
			else if (pNextFocusLayout->GetCount() > 0) {
				focusLevel++;
				pLayout_third->SetVisible(true);
				pNextFocusLayout->GetItemAt(0)->SetFocus();
				//ˢ�½Ǳ�
				refreshSuperscript((CMenuItemUI*)pNextFocusLayout->GetItemAt(0));

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
				CPort *pPort;
				CMenuItemUI* pItem = (CMenuItemUI*)pLayout->GetItemAt(ItemIndex - 2);
				pItem->SetFocus();
				refreshSuperscript(pItem);
			}
			break;
		//-----------------------------------------------
		case VK_DOWN:
			if (pLayout->GetItemAt(ItemIndex + 2)) {
				pLayout->GetItemAt(ItemIndex + 2)->SetFocus();
				refreshSuperscript((CMenuItemUI*)pLayout->GetItemAt(ItemIndex + 2));
			}
			break;
		//-----------------------------------------------
		case VK_RIGHT:
			FocusedItem[1] = pItem;
			ThirdMenuSetFocus(userdata);
			if (m_pm.GetFocus() != FocusedItem[1]) {
				focusLevel++;
				UpdataItemColor();
			}
			else {
				FocusedItem[1] = NULL;
			}
			break;
		//-----------------------------------------------
		case VK_BACK:
			FocusedItem[0]->SetFocus();
			if (_tcscmp(FocusedItem[0]->GetName(), _T("alarmvideo")) == 0) {
				//ˢ�½Ǳ�
				SetAllVirginNum();
			}

			FocusedItem[1] = NULL;
			focusLevel--;
			pLayout_third->SelectItem(0);
			pLayout_third->SetVisible(false);
			UpdataItemColor();
			
			break;
		}
	}
}

void CMyMenuWnd::LabelNotify(TNotifyUI & msg)
{
	CDuiString sName = msg.pSender->GetName();
	CMyLabelUI *pItem = (CMyLabelUI*)msg.pSender;
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

	case VK_LEFT:
		break;

	//--------------------------------------
	case VK_RIGHT:
		break;
	//--------------------------------------

	case VK_UP:
		pParentLayout->GetItemAt(pParentLayout->GetItemIndex(pItem) - 2)->SetFocus();
		break;
	//--------------------------------------
	case VK_RETURN:
		// �������
		if(_tcscmp(sName,_T("sysset_version"))==0){
			VirsionUpdata();
		}
		//�ָ���������
		if (_tcscmp(sName, _T("sysset_reset"))==0) {
			FactoryReset();
		}
		//�Ƿ񴢴�����ͷ��Ƶ
		if (_tcscmp(sName, _T("camera_set_video_save")) == 0) {
			IsStorage(pItem);
		}
		//�Զ���������
		if (_tcscmp(sName, _T("camera_set_shipwatch")) == 0) {
			IsAutoWatch(pItem);
		}
		break;
	}
}

void CMyMenuWnd::CopyFileNotify(TNotifyUI & msg)
{
	//δ��⵽U��
	int HardDriveStatus = DetectHardDriver((list<CRecordFileInfo*>*)msg.lParam);
	if (HardDriveStatus == 0) {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), NULL, _T("δ��⵽U�̣������ԣ�"), NULL, NULL);
	}
	//U������
	else if (HardDriveStatus == 1) {
		PrepareCopy((list<CRecordFileInfo*>*)msg.lParam, msg.wParam);
	}
	//U�̿ղ���
	else {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), _T("U��û���㹻�Ŀռ�, ������ռ�"), _T("���߸���U�̺����ԣ�"), NULL, NULL);
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
			if (_tcscmp(pItem->GetName(), _T("camera_switch")) == 0) {
					//CTime time = CTime::GetCurrentTime();
					if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_camera_switch.xml"), NULL, NULL, NULL, NULL)) {
						pItem->SetValue(false);	
						
						//------------- ����ͷ �� ----------------------//				
						CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
						
						if (pPort) {
							pPort->m_DevConfig.IsCameraOn = 0;
							::SendMessage(((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_SWITCH,  0, (LPARAM)pPort);
							CDBLogger* pLogger = CDBLogger::GetInstance();
							if (pPort->m_DevConfig.IsCameraOn != pItem->GetValue()) {
								pPort->m_DevConfig.IsCameraOn = pItem->GetValue();
								pLogger->LogCameraOnOff(CTime::GetCurrentTime(), pPort);
							}
						}
					}
			}

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
			else {
				//--------------����ͷ �� --------------------//
				CPort* pPort = (CPort*)FocusedItem[1]->GetTag();

				if (pPort) {
					pPort->m_DevConfig.IsCameraOn = 1;
					::SendMessage(((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_SWITCH, 0, (LPARAM)pPort);
					CDBLogger* pLogger = CDBLogger::GetInstance();
					if (pPort->m_DevConfig.IsCameraOn != pItem->GetValue()) {
						pPort->m_DevConfig.IsCameraOn = pItem->GetValue();
						pLogger->LogCameraOnOff(CTime::GetCurrentTime(), pPort);
					}
				}
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
			Print("inx:%d",inx);
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
		if (msg.wParam == 'L') { //����
			if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ListLabelElementUI")) == 0){
				CMyListUI *pSender = (CMyListUI*)msg.pSender;
				if (pSender->Info->status == RECORD_LOCKED) {
					pSender->Info->status = RECORD_SEEN;
				}
				else {
					if (pSender->Info->status == RECORD_NSEEN) {
						//refreshSuperscript(pSender);
					}
					pSender->Info->status = RECORD_LOCKED;
				}
				char sqlStmt[128];
				sprintf_s(sqlStmt, "UPDATE alarm_record SET status = %d WHERE owner = %d AND begin_sec = %d;", pSender->Info->status, pSender->Info->nOwner, pSender->Info->tBegin);
				if (!sqlite.DirectStatement(sqlStmt)) {
					Print("Sql error:%s", sqlStmt);
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

void CMyMenuWnd::VirsionUpdata()
{
	bool UFlash_state = 0;// U��״̬

	if (UFlash_state) {

	}
	else {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), NULL, _T("δ��⵽U�̣������ԣ�"), NULL, NULL);
	}
}

void CMyMenuWnd::FactoryReset()
{
	if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_okcancel.xml"), NULL, _T("ȷ���ָ��������ã�"), NULL, NULL)) {

	}
	else {

	}
}

void CMyMenuWnd::IsStorage(CMyLabelUI *pItem)
{
	CPort* pPort = (CPort*)FocusedItem[1]->GetTag();

	if (pPort == NULL) {
		return ;
	}

	//--------------------- ��Ƶ�洢 ----------------------------------//
	if (pItem->GetValue()) {
		if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_okcancel.xml"), _T("�رմ����������Ƶ���㽫�޷���"), _T("����Ƶ���Ƿ�ȷ���رգ�"), NULL, NULL)) {
			pItem->SetValue(false);
			pPort->m_DevConfig.IsRecordEnabled  = false;
		}
	}
	else {
		pItem->SetValue(true);
		pPort->m_DevConfig.IsRecordEnabled  = true;
	}


	::SendMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_SAVE, 0, (LPARAM)pPort );


	pItem->Invalidate();

}

void CMyMenuWnd::IsAutoWatch(CMyLabelUI *pItem)
{
	CPort* pPort = (CPort*)FocusedItem[1]->GetTag();

	if (pPort == NULL) {
		return;
	}

	if (pItem->GetValue()) {
		if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_okcancel.xml"), _T("�ر�������Զ������󣬸�����ͷ��"), _T("���ᷢ���������Ƿ�ȷ���رգ�"), NULL, NULL)) {
			pItem->SetValue(false);
		}
	}
	else {
		pItem->SetValue(true);
	}

	::SendMessage(((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd,USER_MSG_CAMERA_CONFIG_AWSWITCH, 0, (LPARAM)pPort);

	CDBLogger* pLogger = CDBLogger::GetInstance();
	if (pPort->m_DevConfig.IsAutoWatchEnabled != pItem->GetValue()) {
		pPort->m_DevConfig.IsAutoWatchEnabled = pItem->GetValue();
		pLogger->LogCameraAWOnOff(CTime::GetCurrentTime(), pPort);
	}
	pItem->Invalidate();
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

	pChildLayout1 = (CVerticalLayoutUI*)pLayout->GetItemAt(1); //��������
	pChildLayout2 = (CVerticalLayoutUI*)pLayout->GetItemAt(2); //��������ؼ�����
	if (pChildLayout1->IsVisible() == false) {
		rect = pChildLayout2->GetPos();
		size.cx = 70;
		size.cy = 439;//rect.top + 204;
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
		size.cy = 235;
		pChildLayout2->SetFixedXY(size);
		CMyEditUI *pItem = (CMyEditUI*)(static_cast<CVerticalLayoutUI*>(pLayout->GetItemAt(0))->GetItemAt(2));
		pItem->SetFocus();
		pItem->SetStatus(false);
	}
}

void CMyMenuWnd::ShowAlarmVoiceList(bool Switch)
{
	CContainerUI* pParentLayout = (CContainerUI*)m_pm.FindControl(_T("layout_alarm"));
	CContainerUI* pContain = (CContainerUI*)m_pm.FindControl(_T("alarm_voice"));
	CControlUI* pRbtn = m_pm.FindControl(_T("record_btn"));
	int Width1 = 180, Width2;
	if (pVoice1)
		Width2 = 422;
	else
		Width2 = 348;
	//Width1 = pParentLayout->GetItemAt(0)->GetFixedHeight();
	//Width2 = pParentLayout->GetItemAt(1)->GetFixedHeight();
	//Print("w1:%d,w2:%d ,rbtn:%d",Width1,Width2, pRbtn->GetFixedHeight());
	if (Switch) {
		pParentLayout->SetFixedHeight(Width1 + Width2);
	}
	else {
		pParentLayout->SetFixedHeight(Width1);
	}

	pContain->SetVisible(Switch);
}

void CMyMenuWnd::ThirdMenuSetFocus(CDuiString userdata)
{
	int inx = StrToInt(userdata);
	int sel;
	CVerticalLayoutUI *pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(pLayout_third->GetCurSel());
	//������Ƶ�б�
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
		//��������
		m_pm.FindControl(_T("edit_shipname"))->SetFocus();
	}
	else if (inx >= 7 && inx < 13){
		//���������
		CButtonUI *pEdit;
		sel = pLayout_third->GetCurSel();
		pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(sel);
		pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
		pEdit = (CButtonUI*)pChildLayout->GetItemAt(2); //�����༭��
		pEdit->SetFocus();
	}
	else if (inx == 13){
		//ϵͳ����
		pSysVolum->SetFocus();
	}
	else if (inx == 14){
		//����ʱ��
		static_cast<CButtonUI*>(m_pm.FindControl(_T("time1_hour")))->SetFocus();
	}
	else if (inx == 15){
		//������
		CButtonUI *pAlarmVoiceSwitch;
		sel = pLayout_third->GetCurSel();
		pChildLayout = (CVerticalLayoutUI*)pLayout_third->GetItemAt(sel);
		pChildLayout = (CVerticalLayoutUI*)pChildLayout->GetItemAt(0);
		pAlarmVoiceSwitch = (CButtonUI*)pChildLayout->GetItemAt(2); //����������
		pAlarmVoiceSwitch->SetFocus();
	}
	else if (inx == 16){
		//����ƹ�
		m_pm.FindControl(_T("alarmlight_switch"))->SetFocus();
	}
	else if (inx == 17){
		//�������ؼ�¼
		if (pAwOnOffRecordList->GetCount() > 0) {
			pAwOnOffRecordList->GetItemAt(0)->SetFocus();
			pAwOnOffRecordList->SelectItem(0);
		}
	}
	else if (inx >= 18 && inx < 24) {
		//��Ƶ�б�
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
	//������Ƶ���ܴ�С
	list<CRecordFileInfo*>::iterator iter;
	for (iter = recordInfo->begin(); iter != recordInfo->end(); iter++) {
		totalSize += (*iter)->dlSize;
	}

	//U���Ѳ���
	if (pHardDriver->IsInsert()) {
		//if (recordInfo->empty()) return;
		pHardDriver->GetStatus(&flashSize);
Print("flashSize:%llu",flashSize.mSpaceLeft);
Print("totalSize:%llu",totalSize);
		if (flashSize.mSpaceLeft > totalSize) {
			return 1;
		}
		//�ռ䲻��
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
	//������MenuItem�� ������
	if (ClassName == _T("ButtonUI")) {
		text = focusedItem->GetText();
		CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo_request.xml"), text, NULL, NULL, NULL);
	}
	//�������ļ��б�λ��ʱ���������
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
			text = tbegin.Format("%m��%d��   ") + tbegin.Format("%H:%M:%S") + _T("-") + tend.Format("%H:%M:%S");
		}
		CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo_request.xml"), text, NULL, NULL, NULL);
	}

	if (MSGID_OK == CMsgWnd::MessageBox(m_hWnd, _T("mb_copyvideo.xml"), text, NULL, (LPARAM)recordInfo, type)) {
		CMsgWnd::MessageBox(m_hWnd, _T("mb_ok.xml"), NULL, _T("���Ƴɹ���"), NULL, NULL);
	}

}

void CMyMenuWnd::PlayVideo(WPARAM wParam,LPARAM lParam)
{
	mPlayerWall = new CPlayerWallWnd(_T("playerwall.xml"));
	mPlayerWall->SetMenuWnd(this);
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
	CButtonUI *pRecordBt = (CButtonUI*)m_pm.FindControl(_T("record_btn")); //¼�ư���
	CContainerUI *layout = (CContainerUI*)pRecordBt->GetParent();
	pVoice1 = new CAlarmVoiceListUI(_T("¼��1"),_T("voice1"));
	
	RECT layoutpadding;
	SIZE size_btn = pRecordBt->GetFixedXY();
	int v_height = pDefaultVoice->GetFixedHeight();
	layout->SetFixedHeight(layout->GetFixedHeight()+ 80);
	m_pm.FindControl(_T("layout_alarm"))->SetFixedHeight(604);

	layout->Add(pVoice1);
	pVoice1->SetAttribute(_T("style"), _T("alarm_voice_list"));

	size_btn.cy += 80;
	pRecordBt->SetFixedXY(size_btn);
	pRecordBt->SetText(_T("����¼�Ʊ�����"));
}

void CMyMenuWnd::refreshSuperscript(CMenuItemUI* pItem)
{

	if (_tcscmp(FocusedItem[0]->GetName(), _T("alarmvideo")) == 0) {
		list<CPort*>::iterator iter;
		for (iter = MenuItemVirginNum.begin(); iter != MenuItemVirginNum.end(); iter++) {
			if ((*iter) == (CPort*)(pItem->GetTag()))
				break;
		}
		if (iter == MenuItemVirginNum.end()) {
			MenuItemVirginNum.push_back((CPort*)(pItem->GetTag()));

			//��Ӵ��޸ĽǱ���б��ļ���Ϣ
			CDuiString ListName;
			ListName.Format(_T("video_alarmlist%d"), StrToInt(pItem->GetUserData()) + 1);
			CVideoListUI* pList = (CVideoListUI*)m_pm.FindControl(ListName);
			RefreshListNum(pList);
			//RefreshMenuItemNum();


			//CVideoListUI* pList =
				//RefreshHeadListNum();
			/*					list<CRecordFileInfo*>::iterator iter;
			for (iter = RecordVirginNum.begin(); iter != RecordVirginNum.end(); iter++) {

			}*/
		}
	}


	//CRecordFileInfo* pInfo  = pSender->Info;
	//CVideoListUI::Node* pNode;
	//CMyListUI* pHead;
	//pNode = (CVideoListUI::Node*)pSender->GetTag();
	//pHead = pNode->parent()->data()._pListElement;

	//if (pHead->mhintNumber > 0) {
	//	pHead->mhintNumber--;
	//	pHead->Invalidate();


	//	CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
	//	pPort->m_virginNumber -= 1;
	//	FocusedItem[1]->Invalidate();
	//	SetAllVirginNum();
	//}
}

void CMyMenuWnd::RefreshListNum(CVideoListUI* pList)
{
	CMyListUI* pListLabel;
	char sqlStmt[128];
	for (int i = 0; i < pList->GetCount(); i++) {
		pListLabel = (CMyListUI*)pList->GetItemAt(i);
		RecordVirginNum.push_back(pListLabel);

		if (pListLabel->Info->status == RECORD_NSEEN)
			pListLabel->Info->status = RECORD_SEEN;

		sprintf_s(sqlStmt, "UPDATE alarm_record SET status = %d WHERE owner = %d AND begin_sec = %d;", pListLabel->Info->status, pListLabel->Info->nOwner, pListLabel->Info->tBegin);
		if (!sqlite.DirectStatement(sqlStmt)) {
			Print("Sql error:%s", sqlStmt);
		}
	}
}

void CMyMenuWnd::RefreshMenuItemNum()
{
	list<CPort*>::iterator iter;
	for (iter = MenuItemVirginNum.begin(); iter != MenuItemVirginNum.end(); iter++) {
		(*iter)->m_virginNumber = 0;
	}
	MenuItemVirginNum.empty();
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
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
		if (_tcscmp(msg.pSender->GetName(), _T("camera_set_volume")) == 0) {
			//camera config volume   value:msg.wParam
			CPort* pPort  = (CPort*)FocusedItem[1]->GetTag();
			if (pPort) {
			    int vol  = camera[pPort->GetId()].pVolum->GetValue();
				pPort->m_DevConfig.Volumn  = vol;
				CCommunication::GetInstance()->SetVolume(pPort->m_pCamera,  vol);
				pPort->StoreVolumn();
			}
		}
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
				SetAllVirginNum();
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
				SetAllVirginNum();
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

		case USER_MSG_SYS_VOLUM:
			pSysVolum->SetValue(wParam);
			((CColdEyeApp*)AfxGetApp())->m_SysConfig.volumn = wParam;
			((CColdEyeApp*)AfxGetApp())->StoreSystemConfig();
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
	camera[iInx].pTitle = (CLabelUI*)pChildLayout->GetItemAt(0);		//����
	camera[iInx].pShipname = (CMyEditUI*)pChildLayout->GetItemAt(2);	//����
	pChildLayout = static_cast<CContainerUI*>(pLayout->GetItemAt(2));
	camera[iInx].pSwitch = (CSwitchUI*)pChildLayout->FindSubControl(_T("camera_switch"));//����ͷ����
	camera[iInx].pVolum = (CMySliderUI*)pChildLayout->FindSubControl(_T("camera_set_volume"));//����
	camera[iInx].pSaveVideo = (CMyLabelUI*)pChildLayout->FindSubControl(_T("camera_set_video_save"));//��Ƶ�洢����
	camera[iInx].pAutoWatch = (CMyLabelUI*)pChildLayout->FindSubControl(_T("camera_set_shipwatch"));//�Զ���������
	name.Format(_T("video_list%d"), iInx + 1);
	camera[iInx].pNormalList = (CVideoListUI*)m_pm.FindControl(name);	//��Ƶ�б�
	camera[iInx].pNormalList->SetListElementHeight(m_scale);
	name.Format(_T("video_alarmlist%d"), iInx + 1);
	camera[iInx].pAlarmList = (CVideoListUI*)m_pm.FindControl(name);	//������Ƶ�б�
	camera[iInx].pAlarmList->SetListElementHeight(m_scale);
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
	//MenuItem  �Ǳ�
	list<CPort*>::iterator MenuIteriter;
	for (MenuIteriter = MenuItemVirginNum.begin(); MenuIteriter != MenuItemVirginNum.end(); MenuIteriter++) {
		(*MenuIteriter)->m_virginNumber = 0;
	}
	MenuItemVirginNum.empty();

	//List Head
	list<CMyListUI*>::iterator RecordInfoiter;
	for (RecordInfoiter = RecordVirginNum.begin(); RecordInfoiter != RecordVirginNum.end(); RecordInfoiter++) {
		CVideoListUI::Node* pNode;
		pNode = (CVideoListUI::Node*)(*RecordInfoiter)->GetTag();
		if (pNode->data()._level == 0)
			(*RecordInfoiter)->mhintNumber = 0;
	}
	RecordVirginNum.empty();

	//Pop �Ǳ�
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
	SQLiteStatement* stmt;
	CDuiString sOption,sPage;	

	sprintf_s(sqlStmt, "SELECT COUNT(*) FROM log;");
	stmt = sqlite.Statement(sqlStmt);
	if (stmt->NextRow()) {
		mAwTotalRecord = stmt->ValueInt(0)-1;
		Print("total:%d", mAwTotalRecord);
	}
	else {
		mAwTotalRecord = 0;
	}

	sprintf_s(sqlStmt, "SELECT * FROM log LIMIT 13 OFFSET 1;");
	stmt = sqlite.Statement(sqlStmt);
	while (stmt->NextRow()) {
		CTime time = stmt->ValueInt(0);
		sOption = GetStringOption(stmt->ValueInt(1), stmt->ValueInt(2));
		AddAwOnOffRecord(time, sOption);
	}

	if (mAwTotalRecord % 13) {
		mAwTotalPage = mAwTotalRecord / 13 + 1;
	}
	else {
		mAwTotalPage = mAwTotalRecord / 13;
	}
	if(mAwTotalPage>0)
		mAwPage = 1;
	else mAwPage = 0;

	sPage.Format(_T("��%d/%dҳ"), mAwPage, mAwTotalPage);
	pPage->SetText(sPage);
}

CDuiString CMyMenuWnd::GetStringOption(int option,int tag)
{
	switch (option) {
	case LOG_POWER_ONOFF: //ϵͳ�ϵ�
		if (tag) {
			return _T("ϵͳ�ϵ�");
		}
		else {
			return _T("ϵͳ�ϵ�");
		}
		break;

	//case LOG_SYS_TIME:
	//	return _T("gg");
	//	break;

		//�Զ��������أ�ȫ�֣�
	case LOG_AUTO_WATCH:
		if (tag) {
			return _T("�Զ���������");
		}
		else {
			return _T("�Զ������ر�");
		}
		break;

		//����ͷ��
	case LOG_CAM_ON:
		return _PortName[tag] + _T("����");
		break;

		//����ͷ��
	case LOG_CAM_OFF:
		return _PortName[tag] + _T("�ر�");
		break;

		//����ͷ�Զ���������
	case LOG_CAM_AWON:
		return _PortName[tag] + _T("�Զ���������");
		break;

		//����ͷ�Զ������ر�
	case LOG_CAM_AWOFF:
		return _PortName[tag] + _T("�Զ������ر�");
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
	pListEle->SetFixedHeight(65 * m_scale);
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
			if (stmt->ValueInt(1) == 2) {
				continue;
			}

			sOption = GetStringOption(stmt->ValueInt(1), stmt->ValueInt(2));
			AddAwOnOffRecord(time, sOption);
		}
		if (pAwOnOffRecordList->GetItemAt(0)) {
			pAwOnOffRecordList->GetItemAt(0)->SetFocus();
			pAwOnOffRecordList->SelectItem(0);
			sPage.Format(_T("��%d/%dҳ"), page, mAwTotalPage);
			pPage->SetText(sPage);
		}

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
			pLayout->RemoveAt(order);//�ָ���
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

void CMyMenuWnd::UpdataCameraName(CPort* pPort)
{
	CVerticalLayoutUI* pAlarmVideo = (CVerticalLayoutUI*)m_pm.FindControl(_T("layout_submenu_alarm"));
	CVerticalLayoutUI* pCameraSet = (CVerticalLayoutUI*)m_pm.FindControl(_T("layout_submenu_setting"));
	CVerticalLayoutUI* pVideoObtain = (CVerticalLayoutUI*)m_pm.FindControl(_T("layout_submenu_videoget"));

	//������Ƶ����
	SetMenuItemName(pAlarmVideo, pPort);
	//����ͷ���ð���
	SetMenuItemName(pCameraSet, pPort);
	//��Ƶ��ȡ��������
	SetMenuItemName(pVideoObtain, pPort);
}

void CMyMenuWnd::SetMenuItemName(CVerticalLayoutUI* pLayout, CPort* pPort)
{
	CMenuItemUI* pItem;
	CPort* _pPort;
	for (int i=0; i < pLayout->GetCount(); i += 2) {
		pItem = (CMenuItemUI*)pLayout->GetItemAt(i);
		_pPort = (CPort*)pItem->GetTag();
		if (_pPort) {
			if (_pPort == pPort) {
				pItem->SetText(pPort->GetName());
				break;
			}
		}
	}
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
	if (beginTime > endTime)
		m_pm.FindControl(_T("morrow"))->SetVisible(true);
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
		if (!m_pm.GetFocus()) {
			pLayout_HomeWatch->SelectItem(0);
			pHomeWatchOpen->SetFocus();
			break;
		}

		if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ListLabelElementUI")) == 0) {
			WindowImplBase::OnKeyDown(uMsg, wParam, lParam, bHandled);
		}
		else if (m_pm.GetFocus() == pHomeWatchOpen) {
			if (pLayout_HomeWatch->GetCurSel() == 0) {
				pHome->SetBkColor(ITEM_FOCUS);
				pHome->SetTextColor(0xFF666666);
				pHome->SetFocus();
			}
		}
		else if (_tcscmp(m_pm.GetFocus()->GetClass(), _T("ShipNameItemUI")) == 0) {
			ExpandCameraName();
		}
		else {
			if(FocusedItem[1]){
				KeyDown_VK_BACK();
				UpdataItemColor();
			}
			else {
				WindowImplBase::OnKeyDown(uMsg, wParam, lParam, bHandled);
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
	//---------------��������----------------------
	case 6:
		//�������ڱ༭
		if (pKeyBoard->IsVisible()) {
			pKeyBoard->SetVisible(false);
			m_pm.FindControl(_T("prompt"))->SetVisible(false); //��ʾ��Ϣ
			pShipName->SetFocus();
		}
		else {
			if (ShipNameIsChange()) {
				if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
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
	//---------------����ͷ����----------------------
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		nPort = inx - 6;
		pCameraName = (CMyEditUI*) static_cast<CContainerUI*>(pLayout_third->GetItemAt(inx))->FindSubControl(_T("edit_cameraname"));
		// ����ͷ�������ڱ༭
		if (pCameraName->GetStatus()) {
			ExpandCameraName();
			pCameraName->SetFocus();
		}
		else {
			CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
			if (pPort == NULL) {
				break;
			}


			if (CameraSetIsChange()) {
				if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
					::SendMessage( ((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_NAME, 0, (LPARAM)pPort);
					//::SendMessage(((CColdEyeApp*)AfxGetApp())->GetWallDlg()->m_hWnd, USER_MSG_CAMERA_CONFIG_NAME,)
					camera[nPort - 1].pTitle->SetText(pPort->GetName() + _T("����"));
					UpdataCameraName(pPort);

				}
				else {
					CPort* pPort = (CPort*)FocusedItem[1]->GetTag();
					if (pPort) {
						camera[nPort - 1].pShipname->SetText(pPort->GetName());
						//FillPortConfig(pPort);
					}
				}
			}
		}
		BackTOMenuItem();
		break;
	//----------------ϵͳ����---------------------
	case 13:
		if (SysSetIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
				//��������
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
	//----------------����ʱ��---------------------
	case 14:
		Print("Aw time change");
		if (AwTimeIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
				//���濴��ʱ��
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
	//--------------������----------------------
	case 15:
		if (AlarmVoiceIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
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
	//-------------������---------------------
	case 16:
		if (AlarmLightIsChange()) {
			if (MSGID_OK == CMsgWnd::MessageBox(this->GetHWND(), _T("mb_okcancel.xml"), NULL, _T("ȷ�������������ݣ�"), NULL, NULL)) {
				((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff = pAlarmLight->GetValue();

				((CColdEyeApp*)AfxGetApp())->StoreAlarmLightConfig();
				::SendMessage(((CColdEyeDlg*)AfxGetMainWnd())->m_hWnd,USER_MSG_ALARM_LIGHT,pAlarmLight->GetValue(),NULL);
			}
			else {
				pAlarmLight->SetValue(((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff);
			}
		}
		BackTOMenuItem();
		break; 
	//----------------�Զ��������ؼ�¼---------------

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

int CMyMenuWnd::GetDpi()
{
	return mDPI;
}

float CMyMenuWnd::GetMyScale()
{
	return m_scale;
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

	// �ж��Ƿ���item�Ѿ���Ӧ��port
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

	camera[inx].pTitle->SetText(pPort->GetName()+_T("����"));
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

	//ѡ��Ĭ��  0:Ĭ�ϣ�1¼��
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
