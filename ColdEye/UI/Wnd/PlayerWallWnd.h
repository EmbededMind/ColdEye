#pragma once

#include "UIlib.h"
#include "Wnd\VPlayer.h"
#include "Wnd\AlphaMarkWnd.h"
#include "H264Play.h"
#include "File/RecordFileInfo.h"
#include "MyMenuWnd.h"

#include <list>


//class CPlayerWallWnd;
class CAlphaMarkWnd;
class CMyMenuWnd;

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
	void PreparePlay(WPARAM wParam, CRecordFileInfo* );
	void InitPlayer();
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void InitPlayBtImage();
	void ClosePlayer();
	BOOL StopPlay();
	//bool OnSlow(void* param);
	//bool OnFast(void* param);
	//bool OnPlay(void* param);
	void OnSlow();
	void OnFast();
	void OnPlay();
	void SetMenuWnd(CMyMenuWnd* pWnd);


	CMyMenuWnd* pMenuWnd;
private:
	CDuiString mSkinFileName;

	CVPlayer  *pVPlayer;
	CAlphaMarkWnd *pAlphaMarkWnd;

	// pItem 
	CButtonUI* pSlow;
	CButtonUI* pPlay;
	CButtonUI* pFast;
	CLabelUI* pBeginTime;
	CLabelUI* pEndTime;
	CSliderUI* pSlider;

	DWORD mFileTime;
	long mPort;
	PlayStatus mStatus;
	UINT8 mFastMultiple;
	UINT8 mSlowMultiple;

	LPCTSTR sPlayFocusedImg;
	LPCTSTR sPlayNoFocusImg;
	LPCTSTR sStopFocusedImg;
	LPCTSTR sStopNoFocusImg;

	list<CRecordFileInfo*>* pListInfo;
	UINT8 VoideType;
};