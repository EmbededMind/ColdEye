#include "stdafx.h"
#include "PlayerWallWnd.h"
#include <list>

void __stdcall EOFCallBack(LONG mPort, LONG nUser) 
{
	((CPlayerWallWnd *)nUser)->StopPlay();
}

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
	case USER_MSG_PLAY_START: {

		pListInfo = ((list<CRecordFileInfo*>*)lParam);
		Print("size:%d", pListInfo->size());
		VoideType = wParam;
		InitPlayer();
		}
		break;

	case WM_TIMER:
		OnTimer(uMsg, wParam, lParam, bHandled);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK:
			ClosePlayer();
			break;
		}
		break;
	}
	bHandled = FALSE;
	return 0;
}

void CPlayerWallWnd::InitWindow()
{
	if (pMenuWnd) {
		m_pm.SetDPI(pMenuWnd->GetDpi());//pMenuWnd->GetDpi()
	}
	InitPlayBtImage();
	pSlow = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_slow")));
	//if (pSlow) pSlow->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnSlow);
	pPlay = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_play")));
	//if (pPlay) pPlay->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnPlay);
	pFast = static_cast<CButtonUI*>(m_pm.FindControl(_T("bt_fast")));	
	//if (pFast) pFast->OnNotify += MakeDelegate(this, &CPlayerWallWnd::OnFast);
	pSlider = static_cast<CSliderUI*>(m_pm.FindControl(_T("progress")));

	pBeginTime = static_cast<CLabelUI*>(m_pm.FindControl(_T("time_begin")));
	pEndTime = static_cast<CLabelUI*>(m_pm.FindControl(_T("time_end")));

	if (!pVPlayer) {
		pVPlayer = new CVPlayer(_T("vplayer.xml"));
		pVPlayer->SetDpi(pMenuWnd->GetDpi());
		pVPlayer->Create(m_hWnd, _T("VPlayer"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE, { 0,0,0,0 });
	}
	if (!pAlphaMarkWnd) {
		RECT rt;
		pAlphaMarkWnd = new CAlphaMarkWnd(_T("alpha_mark.xml"));
		pAlphaMarkWnd->SetPlayerWnd(this);
		pAlphaMarkWnd->SetDpi(pMenuWnd->GetDpi());
		pAlphaMarkWnd->Create(m_hWnd, _T("AlphaMarkWnd"), UI_WNDSTYLE_EX_FRAME, WS_EX_WINDOWEDGE, { 0,0,0,0 });
		::GetWindowRect(m_hWnd, &rt);
		int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		ScreenWidth = (ScreenWidth - rt.right) / 2;
		MoveWindow(pAlphaMarkWnd->GetHWND(), ScreenWidth+(rt.left + 707)*(pMenuWnd->GetMyScale()),(rt.top+904)*(pMenuWnd->GetMyScale()), 186*(pMenuWnd->GetMyScale()), 86*(pMenuWnd->GetMyScale()), true);
		pAlphaMarkWnd->ShowWindow(SW_HIDE);
	}
	m_pm.SetFocus(pSlow);
}

void CPlayerWallWnd::PreparePlay(WPARAM wParam, CRecordFileInfo*info)
{
	H264_PLAY_GetPort(&mPort);

	CString path;
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

void CPlayerWallWnd::InitPlayer()
{
	Print("infosize:%d", pListInfo->size());
	if (pListInfo->size() == 0) {
		ClosePlayer();
		return;
	}
	mSlowMultiple = 0;
	mFastMultiple = 0;
	CRecordFileInfo*pInfo = NULL;
	pInfo = pListInfo->front();
	pListInfo->pop_front();
	if (pInfo) {
		PreparePlay(VoideType, pInfo);
		H264_PLAY_Play(mPort, pVPlayer->GetHWND());
		H264_PLAY_SetFileEndCallBack(mPort, EOFCallBack, (LONG)this);
		DWORD PlayTime = H264_PLAY_GetFileTime(mPort);
		CDuiString sTime;
		sTime.Format(_T("%02d:%02d:%02d"), PlayTime / 3600, PlayTime / 60, PlayTime % 60);
		pEndTime->SetText(sTime);
		mStatus = playing;
		pPlay->SetBkImage(sStopNoFocusImg);
		pPlay->SetFocusedImage(sStopFocusedImg);
		SetTimer(m_hWnd, 1, 1000, NULL);
	}
}

LRESULT CPlayerWallWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam) {
	case 1:
		DWORD PlayTime;
		int mProgress = 100*H264_PLAY_GetPlayPos(mPort);  
		PlayTime = H264_PLAY_GetPlayedTime(mPort);
		CDuiString sTime;
		sTime.Format(_T("%02d:%02d:%02d"),PlayTime/3600, PlayTime / 60, PlayTime % 60);
		pBeginTime->SetText(sTime);
		LONG errInfo;
		errInfo = H264_PLAY_GetLastError(mPort);
		Print("progress:%d,%d",mProgress,errInfo);
		pSlider->SetValue(mProgress);
		break;
	}

	bHandled = TRUE;
	return 0;
}

LRESULT CPlayerWallWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (wParam == VK_BACK) {
		WindowImplBase::OnKeyDown(uMsg, wParam, lParam, bHandled);
	}
	else {
		switch (wParam) {
		case VK_LEFT:
			OnSlow();
			break;

		case VK_RIGHT:
			OnFast();
			break;

		case VK_RETURN:
			OnPlay();
			break;
		}

		//m_pm.SendNotify(m_pm.GetFocus(), DUI_MSGTYPE_KEYDOWN, wParam, lParam);
	}

	return LRESULT();
}

void CPlayerWallWnd::InitPlayBtImage()
{
	sPlayFocusedImg = _T("file='image/play_focused.png'");
	sPlayNoFocusImg = _T("file='image/play.png'");
	sStopFocusedImg = _T("file='image/pause_focused.png'");
	sStopNoFocusImg = _T("file='image/pause.png'");
}

void CPlayerWallWnd::ClosePlayer()
{
	KillTimer(m_hWnd, 1);
	H264_PLAY_Stop(mPort);
	H264_PLAY_CloseFile(mPort);
	H264_PLAY_FreePort(mPort);
	pAlphaMarkWnd->Close();
	pVPlayer->Close();	
	pListInfo->clear();
	Close();
}


BOOL CPlayerWallWnd::StopPlay()
{
	KillTimer(m_hWnd,1);
	H264_PLAY_Stop(mPort);
	H264_PLAY_CloseFile(mPort);
	H264_PLAY_FreePort(mPort);
	pSlider->SetValue(100);
	pAlphaMarkWnd->ShowWindow(false);
	InitPlayer();
	return 0;
}

//bool CPlayerWallWnd::OnSlow(void * param)
//{
//	TNotifyUI* pMsg = (TNotifyUI*)param;
//	Print("%S", pMsg->sType);
//
//	//键盘按键事件
//	if (pMsg->sType == DUI_MSGTYPE_KEYDOWN) {
//		switch (pMsg->wParam) {
//		case VK_RIGHT:
//			pPlay->SetFocus();
//			break;
//
//		case VK_RETURN:
//			if (mStatus != pause){
//				int play_slow;
//				play_slow = H264_PLAY_Slow(mPort);
//				if (mSlowMultiple < 4)
//					mSlowMultiple++;
//				Print("play_slow:%d", play_slow);
//				mStatus = slow_fast;
//				pAlphaMarkWnd->ShowWindow(SW_SHOW);
//				::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_SLOW, mSlowMultiple, NULL);
//			}
//			break;
//		}
//	}
//	return true;
//}
//
//bool CPlayerWallWnd::OnFast(void * param)
//{
//	TNotifyUI* pMsg = (TNotifyUI*)param;
//	if (pMsg->sType == DUI_MSGTYPE_KEYDOWN) {
//		switch (pMsg->wParam) {
//		case VK_LEFT:
//			pPlay->SetFocus();
//			break;
//
//		case VK_RETURN:
//			if (mStatus != pause) {
//				int play_fast;
//				play_fast = H264_PLAY_Fast(mPort);
//				if (mFastMultiple < 4)
//					mFastMultiple++;
//				Print("play_fast:%d",play_fast);
//				mStatus = slow_fast;
//				::ShowWindow(pAlphaMarkWnd->GetHWND(), true);
//				::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_FAST, mFastMultiple, NULL);
//			}
//			break;
//		}
//	}
//
//	return true;
//}
//
//bool CPlayerWallWnd::OnPlay(void * param)
//{
//	TNotifyUI* pMsg = (TNotifyUI*)param;
//
//	if (pMsg->sType == DUI_MSGTYPE_KEYDOWN) {
//		switch (pMsg->wParam) {
//		case VK_LEFT:
//			pSlow->SetFocus();
//			break;
//
//		case VK_RIGHT:
//			pFast->SetFocus();
//			break;
//
//		case VK_RETURN:
//			if (mStatus == pause){
//				//暂停时恢复播放
//				if (!H264_PLAY_Pause(mPort, 0)) {
//					long err = H264_PLAY_GetLastError(mPort);
//					printf("play err = %d\n", err);
//				}
//				else {
//					mStatus = playing;
//				}
//			}
//			else if (mStatus == playing) {
//				//暂停播放
//				if (!H264_PLAY_Pause(mPort, 1)) {
//					long err = H264_PLAY_GetLastError(mPort);
//					printf("play err = %d\n", err);
//				}
//				else {
//					mStatus = pause;
//				}
//			}
//			else if(mStatus == slow_fast){
//				//恢复正常播放速度
//				if (!H264_PLAY_Play(mPort, pVPlayer->GetHWND())){
//					long err = H264_PLAY_GetLastError(mPort);
//					printf("play err = %d\n", err);
//				}
//				else {
//					mStatus = playing;
//					mSlowMultiple = 0;
//					mFastMultiple = 0;
//				}
//			}
//
//			if (mStatus == playing) {
//				SetTimer(m_hWnd, 1, 1000, NULL);
//				::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
//				pPlay->SetBkImage(sStopNoFocusImg);
//				pPlay->SetFocusedImage(sStopFocusedImg);
//			}
//			else {
//				pPlay->SetBkImage(sPlayNoFocusImg);
//				pPlay->SetFocusedImage(sPlayFocusedImg);
//				KillTimer(m_hWnd, 1);
//			}
//			break;
//		}
//	}
//	return true;
//}

void CPlayerWallWnd::OnSlow()
{
	if (mStatus != pause){
		if (mFastMultiple != 0) {
			H264_PLAY_Play(mPort, pVPlayer->GetHWND());
			mFastMultiple = 0;
		}
		pSlow->SetFocus();
		int play_slow;
		play_slow = H264_PLAY_Slow(mPort);
		if (mSlowMultiple < 4)
			mSlowMultiple++;
		else {
			mSlowMultiple = 0;
			H264_PLAY_Play(mPort, pVPlayer->GetHWND());
			mStatus = playing;
			::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
			return;
		}
		Print("play_slow:%d", play_slow);
		mStatus = slow_fast;
		pAlphaMarkWnd->ShowWindow(SW_SHOW);
		::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_SLOW, mSlowMultiple, NULL);
	}
}

void CPlayerWallWnd::OnFast()
{
	if (mStatus != pause) {
		if (mSlowMultiple != 0) {
			H264_PLAY_Play(mPort, pVPlayer->GetHWND());
			mSlowMultiple = 0;
		}
		pFast->SetFocus();
		int play_fast;
		play_fast = H264_PLAY_Fast(mPort);
		if (mFastMultiple < 4)
			mFastMultiple++;
		else {
			mFastMultiple = 0;
			H264_PLAY_Play(mPort, pVPlayer->GetHWND());
			mStatus = playing;
			::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
			return;
		}
		Print("play_fast:%d",play_fast);
		mStatus = slow_fast;
		::ShowWindow(pAlphaMarkWnd->GetHWND(), true);
		::SendMessage(pAlphaMarkWnd->GetHWND(), USER_MSG_PLAY_FAST, mFastMultiple, NULL);
	}
}

void CPlayerWallWnd::OnPlay()
{
	pPlay->SetFocus();
	mSlowMultiple = 0;
	mFastMultiple = 0;
	if (mStatus == pause){
		//暂停时恢复播放
		if (!H264_PLAY_Pause(mPort, 0)) {
			long err = H264_PLAY_GetLastError(mPort);
			printf("play err = %d\n", err);
		}
		else {
			mStatus = playing;
		}
	}
	else if (mStatus == playing) {
		//暂停播放
		if (!H264_PLAY_Pause(mPort, 1)) {
			long err = H264_PLAY_GetLastError(mPort);
			printf("play err = %d\n", err);
		}
		else {
			mStatus = pause;
		}
	}
	else if(mStatus == slow_fast){
		//恢复正常播放速度
		H264_PLAY_Play(mPort, pVPlayer->GetHWND());
		mSlowMultiple = 0;
		mFastMultiple = 0;
		H264_PLAY_Pause(mPort, 1);
		::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
		mStatus = pause;
	}
	
	if (mStatus == playing) {
		SetTimer(m_hWnd, 1, 1000, NULL);
		::ShowWindow(pAlphaMarkWnd->GetHWND(), false);
		pPlay->SetBkImage(sStopNoFocusImg);
		pPlay->SetFocusedImage(sStopFocusedImg);
	}
	else {
		pPlay->SetBkImage(sPlayNoFocusImg);
		pPlay->SetFocusedImage(sPlayFocusedImg);
		KillTimer(m_hWnd, 1);
	}
}

void CPlayerWallWnd::SetMenuWnd(CMyMenuWnd * pWnd)
{
	pMenuWnd = pWnd;
}
