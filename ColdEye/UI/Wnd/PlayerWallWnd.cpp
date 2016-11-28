#include "stdafx.h"
#include "PlayerWallWnd.h"


CPlayerWallWnd::CPlayerWallWnd()
	:mStatus(stop),
	mSlowMultiple(0),
	mFastMultiple(0)
{
}

CPlayerWallWnd::CPlayerWallWnd(CDuiString skinName)
{
	mSkinFileName = skinName;
}

CPlayerWallWnd::~CPlayerWallWnd()
{
}

LPCTSTR CPlayerWallWnd::GetWindowClassName() const
{
	return _T("PlayerWallWnd");
}

CDuiString CPlayerWallWnd::GetSkinFile()
{
	return mSkinFileName;
}

LRESULT CPlayerWallWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (uMsg){
	case USER_MSG_PLAY_START:
		PreparePlay(wParam, lParam);
		H264_PLAY_Play(mPort, pVPlayer->GetHWND());
		mStatus = playing;
		SetTimer(m_hWnd, 1, 1000, NULL);
		break;

	case WM_TIMER:
		OnTimer(uMsg, wParam, lParam, bHandled);
		break;
	}
	return LRESULT();
}

void CPlayerWallWnd::InitWindow()
{
	pSlow = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_slow")));
	if (pSlow) pSlow->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnSlow);
	pPlay = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_play")));
	if (pPlay) pPlay->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnPlay);
	pFast = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_fast")));
	if (pFast) pFast->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnFast);
	pSlider = static_cast<CSliderUI*>(m_pm.FindControl(_T("progress")));

	if (!pVPlayer) {
		pVPlayer = new CVPlayer(_T("vplayer.xml"));
		pVPlayer->Create(m_hWnd, _T("VPlayer"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE, { 0,0,0,0 });
	}
	if (!pAlphaMarkWnd) {
		RECT rt;
		pAlphaMarkWnd = new CAlphaMarkWnd(_T("alpha_mark.xml"));
		pAlphaMarkWnd->Create(m_hWnd, _T("AlphaMarkWnd"), WS_POPUP | WS_VISIBLE, WS_EX_WINDOWEDGE, { 0,0,0,0 });
		::GetWindowRect(m_hWnd, &rt);
		MoveWindow(pAlphaMarkWnd->GetHWND(), rt.left+707,rt.top+704, 186, 86, true);
		pAlphaMarkWnd->ShowWindow(false);
	}
}

void CPlayerWallWnd::PreparePlay(WPARAM wParam, LPARAM lParam)
{
	H264_PLAY_GetPort(&mPort);

	CString path;
	CRecordFileInfo *info = (CRecordFileInfo*)lParam;
	if (wParam == RECORD_NORMAl)
	{
		path = _T(NORMAL_RECORD_PATH);
	}
	else
	{
		path = _T(ALARM_RECORD_PATH);
	}
	CString tmp;
	CTime begintime = info->tBegin;
	tmp.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), info->nOwner, begintime.GetYear(), begintime.GetMonth(), begintime.GetDay(),
		begintime.GetHour(), begintime.GetMinute(), begintime.GetSecond());
	path += tmp; Print("%S", path);
	USES_CONVERSION;
	const char * ptr = T2CA(path);
	H264_PLAY_OpenFile(mPort, ptr);
}

LRESULT CPlayerWallWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam) {
	case 1:
		int mProgress = H264_PLAY_GetPlayedTime(mPort);
		LONG errInfo;
		errInfo = H264_PLAY_GetLastError(mPort);
		Print("progress:%d,%d",mProgress,errInfo);
		pSlider->SetValue(mProgress);
		break;
	}

	bHandled = TRUE;
	return 0;
}

bool CPlayerWallWnd::OnSlow(void * param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == DUI_MSGTYPE_CLICK) {
		if (mStatus != pause){
			int play_slow;
			play_slow = H264_PLAY_Slow(mPort);
			if (mSlowMultiple < 4)
				mSlowMultiple++;
			Print("play_slow:%d", play_slow);
			mStatus = slow_fast;
			::ShowWindow(pAlphaMarkWnd->GetHWND(), true);
			::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_SLOW, mSlowMultiple, NULL);
		}
	}
	return true;
}

bool CPlayerWallWnd::OnFast(void * param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == DUI_MSGTYPE_CLICK) {
		if (mStatus != pause) {
			int play_fast;
			play_fast = H264_PLAY_Fast(mPort);
			if (mFastMultiple < 4);
				mFastMultiple++;
			Print("play_fast:%d",play_fast);
			mStatus = slow_fast;
			::ShowWindow(pAlphaMarkWnd->GetHWND(), true);
			::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_FAST, mFastMultiple, NULL);
		}
	}
	return true;
}

bool CPlayerWallWnd::OnPlay(void * param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == DUI_MSGTYPE_CLICK) {
			if (mStatus == pause){
				//暂停时恢复播放
				if (!H264_PLAY_Pause(mPort, 0)) {
					long err = H264_PLAY_GetLastError(mPort);
					printf("play err = %d\n", err);
				}
				mStatus = playing;
			}
			else if (mStatus == playing) {
				//暂停播放
				if (!H264_PLAY_Pause(mPort, 1)) {
						long err = H264_PLAY_GetLastError(mPort);
						printf("play err = %d\n", err);
					}
					mStatus = pause;
			}
			else if(mStatus == slow_fast){
				//恢复正常播放速度
				if (!H264_PLAY_Play(mPort, pVPlayer->GetHWND())){
					long err = H264_PLAY_GetLastError(mPort);
					printf("play err = %d\n", err);
				}
				mStatus = playing;
				mSlowMultiple = 0;
				mFastMultiple = 0;
			}

			if (mStatus == playing)
				::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
	}
	return true;
}
