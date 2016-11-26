#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"

#include "Control\MyEditUI.h"
#include "Control\CameraSwitchUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"
#include "Control\MenuItemUI.h"
#include "Control\VideoListUI.h"
#include "Control\SwitchRecordListUI.h"
#include "Control\TimeButtonUI.h"

#include "Wnd\Player.h"


#include "Device\PortManager.h"


#include <list>

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
		CCameraSwitchUI *pSwitch;	//开关
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
	void OnFinalMessage(HWND hWnd);

	// Item Notify
	void SliderNotify(TNotifyUI & msg);
	void EditNotify(TNotifyUI &msg);
	void MenuItemNotify(TNotifyUI &msg);
	void LabelNotify(TNotifyUI &msg);
	void ExpandCameraName();
	void ThirdMenuSetFocus(CDuiString);
	void Notify(TNotifyUI& msg);
	void OnLClick(CControlUI* pControl);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DeleteAlarmCtl(CameraInfo);
	void DeleteCameraSetCtl(CameraInfo);
	void DeleteVideoObtain(CameraInfo);

	void AddWatchRecord(SwtichRecord);

	int InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData);
	void AddCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void GetCameraItem(CVerticalLayoutUI *pLayout);
	void CameraInfoInit(CameraInfo cameraInfo);
	CameraInfo GetCameraSetInfo(int id);

	void SetWatchTime(DWORD beginTime, DWORD endTime);
	void GetWatchTime(DWORD* pBegining, DWORD* pEnd);


	bool CameraSetIsChange();
	bool ShipNameIsChange();
	bool SysSetIsChange();
	bool AwTimeIsChange();


	void MyMessageBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
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

	void  InitRecordFile(list<CRecordFileInfo*>* pList);
	void  InitAlarmFile(list<CRecordFileInfo*>* pList);

	void   FillPortConfig(CPort* pPort);

	CTabLayoutUI *pLayout_third;
	CTabLayoutUI *pLayout_Menuitem;
	CTabLayoutUI *pLayout_PopMenu;
	CContainerUI *pKeyBoard;	//键盘控件
	CMyEditUI * pShipName;		//船名
	CMySliderUI* pSysVolum;		//系统音量
	CMySliderUI* pSysLight;		//系统亮度
	CMyLabelUI* pHostModel;		//主机型号
	CTimeButtonUI* pAwTime[4];  //自动看船
	CButtonUI *FocusedItem[2];

	CPlayer	 *mPlayer;
};