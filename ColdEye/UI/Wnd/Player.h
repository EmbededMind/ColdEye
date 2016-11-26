#pragma once

#include "UIlib.h"
#include "H264Play.h"
#include "File/RecordFileInfo.h"

using namespace std;

using namespace DuiLib;

class CPlayer :
	public WindowImplBase{

public:
	CPlayer();
	CPlayer(CDuiString);
	~CPlayer();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void InitWindow();
private:
	CDuiString sXml;
	CButtonUI *pButton;
	CButtonUI mPlayer;
};