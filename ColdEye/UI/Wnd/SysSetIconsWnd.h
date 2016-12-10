#pragma once

#include "UIlib.h"


using namespace std;
using namespace DuiLib;

class CSysSetIconsWnd
	:public WindowImplBase {

public:
	CSysSetIconsWnd();
	CSysSetIconsWnd(CDuiString);
	~CSysSetIconsWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
	void InitWindow();
	void SetDpi(int dpi);
private:
	CDuiString mSkinFileName;

	CButtonUI* pVolume;
	CButtonUI* pLight;
	int mshowTime;
	int mDpi;
};