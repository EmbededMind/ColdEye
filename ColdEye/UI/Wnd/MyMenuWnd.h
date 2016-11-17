#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"
#include "Control\MyEditUI.h"
#include "Control\CameraSwitchUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"

using namespace DuiLib;

class CMyMenuWnd :
	public WindowImplBase
{
public:
	struct Cameraset {
		CMyEditUI *pShipname;
		CCameraSwitchUI *pSwitch;
		CMySliderUI *pVolum;
		CMyLabelUI *pVideoSave;
		CMyLabelUI *pAutoWatch;
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
	void AddCamear(UINT8 iInx);
	void SetCameraInfo();
private:
	Cameraset cameraset[6];
};