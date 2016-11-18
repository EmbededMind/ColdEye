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
	UINT8 id;				   //����ͷID
	bool IsActivate;           //������Ƿ񿪻�
	bool IsVideoRecordEnabled; //��Ƶ�洢����
	bool IsAutoWatchEnabled;   //�Զ���������
	VIDEO_DIRECTION VideoDir;  //��Ƶ����
	UCHAR Volumn;              //����
	CString Name;				   //���������
}CameraInfo;


class CMyMenuWnd :
	public WindowImplBase
{
public:

	struct CameraItem {
		UINT8 id;	//����ӿ�
		CMyEditUI *pShipname;	//����
		CCameraSwitchUI *pSwitch;	//����
		CMySliderUI *pVolum;	//����
		CMyLabelUI *pVideoSave;	//��Ƶ��������
		CMyLabelUI *pAutoWatch;	//�Զ���������
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