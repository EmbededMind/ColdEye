#pragma once

#include "UIlib.h"
#include "H264Play.h"
#include "File\RecordFileInfo.h"

using namespace std;
using namespace DuiLib;

class CAlphaMarkWnd
	:public WindowImplBase {

public:
	CAlphaMarkWnd();
	CAlphaMarkWnd(CDuiString);
	~CAlphaMarkWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
	void InitWindow();

private:
	CDuiString mSkinFileName;

	CButtonUI* pBt_fast;
	CButtonUI* pBt_slow;
};