#pragma once

#include "UIlib.h"
#include "H264Play.h"
#include "File\RecordFileInfo.h"
//#include "PlayerWallWnd.h"

//class CPlayerWallWnd;

using namespace std;

using namespace DuiLib;

class CVPlayer
	:public WindowImplBase {

public:
	CVPlayer();
	CVPlayer(CDuiString);
	~CVPlayer();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	void SetDpi(int dpi);
	virtual void InitWindow();
 
private:
	CDuiString mSkinFileName;
	int mDpi;
};