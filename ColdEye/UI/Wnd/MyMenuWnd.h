#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"
#include "Control\MyEditUI.h"
#include "Control\CameraSwitchUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"
#include "Device\Camera.h"

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


class CMyMenuWnd :
	public WindowImplBase
{
public:

	struct CameraItem {
		UINT8 id;	//物理接口
		CMyEditUI *pShipname;	//船名
		CCameraSwitchUI *pSwitch;	//开关
		CMySliderUI *pVolum;	//音量
		CMyLabelUI *pVideoSave;	//视频保存设置
		CMyLabelUI *pAutoWatch;	//自动看船设置
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
	void CameraSetItemInit(CVerticalLayoutUI *pLayout);
	void AddCamear(CameraInfo cameraInfo);
	void SetCameraInfo();
private:
	CameraItem cameraset[6];
};