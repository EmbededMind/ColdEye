#pragma once

#include "UIlib.h"
#include "Wnd\VPlayer.h"
#include "Wnd\AlphaMarkWnd.h"
#include "H264Play.h"
#include "File/RecordFileInfo.h"

class CAlphaMarkWnd;
using namespace std;

using namespace DuiLib;

enum PlayStatus
{
	stop = 0,
	playing,
	pause,
	slow_fast
};

class CPlayerWallWnd :
	public WindowImplBase{

public:
	CPlayerWallWnd();
	CPlayerWallWnd(CDuiString);
	~CPlayerWallWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void InitWindow();
	void PreparePlay(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	bool OnSlow(void* param);
	bool OnFast(void* param);
	bool OnPlay(void* param);

private:
	CDuiString mSkinFileName;

	CVPlayer  *pVPlayer;
	CAlphaMarkWnd *pAlphaMarkWnd;

	// pItem 
	CButtonUI* pSlow;
	CButtonUI* pPlay;
	CButtonUI* pFast;
	CSliderUI* pSlider;

	DWORD mFileTime;
	long mPort;
	PlayStatus mStatus;
	UINT8 mFastMultiple;
	UINT8 mSlowMultiple;
};