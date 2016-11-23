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


#include "Device\Camera.h"
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
		CMyEditUI *pShipname;	//船名
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
	void Notify(TNotifyUI& msg);
	void OnLClick(CControlUI* pControl);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void AddCamear(CameraInfo);
	void AddAlarmCtl(CameraInfo);
	void AddCameraSetCtl(CameraInfo);
	void AddVideoObtain(CameraInfo);
	void DeleteAlarmCtl(CameraInfo);
	void DeleteCameraSetCtl(CameraInfo);
	void DeleteVideoObtain(CameraInfo);
	void AddWatchRecord(SwtichRecord);

	int InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData);
	void Relationship(CVerticalLayoutUI *pLayout,CMenuItemUI* pMenuItem);
	void AddCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void GetCameraItem(CVerticalLayoutUI *pLayout);
	void CameraInfoInit(CameraInfo cameraInfo);
	CameraInfo GetCameraSetInfo(int id);

	//void FindRecordFile();

	void MyMessageBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	//test
	CameraInfo cameraInfo;
	SwtichRecord recordInfo;
	CameraItem camera[6];
private:



	void  AddMenuItem(CPort* pPort, CDuiString layoutName, int baseData);
	void  AddAlarmMenuItem(CPort* pPort);
	void  AddVideoObtainItem(CPort* pPort);



	void  InitRecordFile(list<CRecordFileInfo*>* pList);
	void  InitAlarmFile(list<CRecordFileInfo*>* pList);
};