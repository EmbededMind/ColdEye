#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"

#include "Control\MyEditUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"
#include "Control\MenuItemUI.h"
#include "Control\VideoListUI.h"
#include "Control\SwitchUI.h"
#include "Control\AlarmVoiceListUI.h"
#include "Control\AWOnOffListLabelUI.h"
#include "Control\TimeButtonUI.h"

#include "Wnd\PlayerWallWnd.h"


#include "Device\PortManager.h"
#include "ExHardDrive\ExHardDrive.h"


#include <list>

//==============BkColor=====================
#define LAYOUT_POP_FOCUSED		0xFFFFFFFF
#define	LAYOUT_POP_NOFOCUS		0xFFE6E6EF

#define LAYOUT_MENUITEM_FOCUSED		 0xFFFFFFFF
#define LAYOUT_MENUITEM_NOFOCUS		 0xFFE6E6EF
#define LAYOUT_MNEUITEM_SEL_NOFOCUS	 0xFFEFEFF4

#define LAYOUT_THIRD_NOFOUCS		 0xFFEFEFF4
#define LAYOUT_THIRD_FOCUSED		 0xFFFFFFFF

#define ITEM_FOCUS					 0xFF4198FE
#define POPMENU_SEL_NOFOCUS			 0xFF47688F
#define MENUITEM_SEL_NOFOCUS		 0xFF4178B7

using namespace std;


using namespace DuiLib;

typedef struct {
	UINT8 id;				   //摄像头ID
	bool IsActivate;           //摄像机是否开机
	bool IsVideoRecordEnabled; //视频存储开关
	bool IsAutoWatchEnabled;   //自动看船开关
	VIDEO_DIRECTION VideoDir;  //视频方向
	UCHAR Volumn;              //音量
	CString Name;				   //摄像机名称
}CameraInfo;


typedef struct {
	CTime time;
	CDuiString record_type;
} SwtichRecord;


class CMyMenuWnd :
	public WindowImplBase
{
public:

	struct CameraItem {
		UINT8 id;	//物理接口
		CLabelUI *pTitle;	//标题
		CMyEditUI *pShipname;	//摄像头名称
		CSwitchUI *pSwitch;	//开关
		CMySliderUI *pVolum;	//音量
		CMyLabelUI *pSaveVideo;	//视频保存设置
		CMyLabelUI *pAutoWatch;	//自动看船设置
		CVideoListUI *pNormalList; //视频列表
		CVideoListUI *pAlarmList;	//报警视频列表
	};

public :
	CMyMenuWnd();
	~CMyMenuWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	void InitWindow();


	bool OnHomeWatch(void* param);
	// BkColor
	void UpdataItemColor();

	// Item Notify
	void SliderNotify(TNotifyUI& msg);
	void EditNotify(TNotifyUI& msg);
	void MenuItemNotify(TNotifyUI& msg);
	void LabelNotify(TNotifyUI& msg);
	void CopyFileNotify(TNotifyUI& msg);
	void RecordVoiceNotify(TNotifyUI& msg);
	void AlarmVoiceListNotify(TNotifyUI& msg);
	void SwitchNotify(TNotifyUI& msg);
	void ListLabelNotify(TNotifyUI& msg);

	void VirsionUpdata();
	void FactoryReset();
	void IsStorage(CMyLabelUI *pItem);
	void IsAutoWatch(CMyLabelUI *pItem);
	void ExpandCameraName();
	void ShowAlarmVoiceList(bool);
	void ThirdMenuSetFocus(CDuiString);
	int DetectHardDriver(list<CRecordFileInfo*>*);
	void PrepareCopy(list<CRecordFileInfo*>*,UINT8);
	void PlayVideo(WPARAM wParam, LPARAM lParam);
	void RecordVoice();
	void AddAlarmVoice();
	void refreshSuperscript(CMyListUI*); //刷新角标
	void Notify(TNotifyUI& msg);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetAllVirginNum();
	void DeleteAlarmCtl(CameraInfo);
	void DeleteCameraSetCtl(CameraInfo);
	void DeleteVideoObtain(CameraInfo);

	//自动看船开关记录
	void InitAwOnOffRecord();
	CDuiString GetStringOption(int option,int tag);
	void AddAwOnOffRecord(CTime, CDuiString);
	void AwPage(int page);
	void AwOnOffRecordNextPage();
	void AwOnOffRecordLastPage();

	int InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData);
	void AddCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void GetCameraItem(CVerticalLayoutUI *pLayout);
	void CameraInfoInit(CameraInfo cameraInfo);
	CameraInfo GetCameraSetInfo(int id);
	void UpdataCameraName(CPort*);	//更新船名
	void SetMenuItemName(CVerticalLayoutUI* pLayout, CPort* pPort);


	void SetWatchTime(DWORD beginTime, DWORD endTime);
	void GetWatchTime(DWORD* pBegining, DWORD* pEnd);


	bool CameraSetIsChange();
	bool ShipNameIsChange();
	bool SysSetIsChange();
	bool AwTimeIsChange();
	bool AlarmVoiceIsChange();
	bool AlarmLightIsChange();

	virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void KeyDown_VK_BACK();

	CameraInfo cameraInfo;
	SwtichRecord recordInfo;
	CameraItem camera[6];
private:

	CMenuItemUI*  AddMenuItem(CPort* pPort, CDuiString layoutName, int baseData);

	void  AddAlarmMenuItem(CPort* pPort);
	void  AddVideoObtainMenuItem(CPort* pPort);
	//void  AddCameraConfigMenuItem(CCamera* pCamera);
	void  AddPortConfigMenuItem(CPort * pPort);

	void  DeleteMenuItem(CPort* pPort, CDuiString layoutname, int basedata);
	void  DeleteAlarmMenuItem(CPort* pPort);
	void  DeleteVideoObtainMenuItem(CPort* pPort);
	void  DeletePortConfigMenuItem(CPort* pPort);

	void  InitRecordFile(list<CRecordFileInfo*>* pList);
	void  InitAlarmFile(list<CRecordFileInfo*>* pList);

	void   FillPortConfig(CPort* pPort);
	void   InitAlarmVoice();
	void   BackTOMenuItem();

	CMenuItemUI* pAlarmItem[6];

	CTabLayoutUI *pLayout_third;
	CTabLayoutUI *pLayout_Menuitem;
	CTabLayoutUI *pLayout_PopMenu;
	CTabLayoutUI* pLayout_HomeWatch;
	CContainerUI *pKeyBoard;	//键盘控件
	CMyEditUI * pShipName;		//船名
	CMySliderUI* pSysVolum;		//系统音量
	CMySliderUI* pSysLight;		//系统亮度
	CMyLabelUI* pHostModel;		//主机型号
	CTimeButtonUI* pAwTime[4];  //自动看船
	CSwitchUI* pAlmVicSwitch; //报警音开关
	CAlarmVoiceListUI* pDefaultVoice;	//默认报警音
	CAlarmVoiceListUI* pVoice1;		//录警音
	CSwitchUI*	pAlarmLight;		//报警灯光
	CListUI*	pAwOnOffRecordList;		//自动看船开关记录列表
	CLabelUI* pPage;		//页码
	CButtonUI* pHome;
	CButtonUI* pHomeWatchOpen;	//回家看船开启

	CButtonUI *FocusedItem[2];
	UINT8 focusLevel;
	UINT8 mAlarmVoiceSel;
	int mAwTotalPage;
	int mAwPage;
	CPlayerWallWnd	*mPlayerWall;
};