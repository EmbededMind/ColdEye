#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"
#include "Control\MyEditUI.h"
#include "Control\CameraSwitchUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"
#include "Device\Camera.h"
#include "Control\MenuItemUI.h"
#include "Control\VideoListUI.h"
#include "Control\SwitchRecordListUI.h"

using namespace DuiLib;

typedef struct {
	UINT8 id;				   //����ͷID
	bool IsActivate;           //������Ƿ񿪻�
	bool IsVideoRecordEnabled; //��Ƶ�洢����
	bool IsAutoWatchEnabled;   //�Զ���������
	VIDEO_DIRECTION VideoDir;  //��Ƶ����
	UCHAR Volumn;              //����
	CString Name;				   //���������
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
		UINT8 id;	//����ӿ�
		CLabelUI *pTitle;	//����
		CMyEditUI *pShipname;	//����
		CCameraSwitchUI *pSwitch;	//����
		CMySliderUI *pVolum;	//����
		CMyLabelUI *pSaveVideo;	//��Ƶ��������
		CMyLabelUI *pAutoWatch;	//�Զ���������
		CVideoListUI *pNormalList; //��Ƶ�б�
		CVideoListUI *pAlarmList;	//������Ƶ�б�
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

	void AddCamera(CCamera* pCamera);
	void AddCamear(CameraInfo);
	void AddAlarmCtl(CameraInfo);
	void AddCameraSetCtl(CameraInfo);
	void AddVideoObtain(CameraInfo);
	void DeleteAlarmCtl(CameraInfo);
	void DeleteCameraSetCtl(CameraInfo);
	void DeleteVideoObtain(CameraInfo);
	void ADDWatchRecord(SwtichRecord);


	


	int InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData);
	void Relationship(CVerticalLayoutUI *pLayout,CMenuItemUI* pMenuItem);
	void AddCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void GetCameraItem(CVerticalLayoutUI *pLayout);
	void CameraInfoInit(CameraInfo cameraInfo);
	CameraInfo GetCameraSetInfo(int id);

	void MyMessageBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	//test
	CameraInfo cameraInfo;
	SwtichRecord recordInfo;
private:
	CameraItem camera[6];


	void  AddAlarmMenuItem(CCamera* pCamera);
};