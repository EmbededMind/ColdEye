#pragma once

#include "UIlib.h"
#include "H264Play.h"
#include "File\RecordFileInfo.h"

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
 
private:
	CDuiString mSkinFileName;
};