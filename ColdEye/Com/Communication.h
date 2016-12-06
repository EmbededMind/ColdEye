#pragma once
#include "IStatus.h"
#include "FreeState.h"
#include "AlarmState.h"
#include "CameraTalkState.h"
#include "HostTalkState.h"
#include "WaitReply.h"
#include "Device\Camera.h"

class CCommunication
{
private:
	IState *FreeState;
	IState *AlarmState;
	IState *CameraTalkState;
	IState *HostTalkState;
	IState *WaitReplyState;

	IState *mState;
	IState *mOldState;

	UINT mTimerID;
public:
	CCamera *mPdev;
	LONG mHandle;
	static CCommunication* GetInstance()
	{
		static CCommunication instance;
		return &instance;
	}
	static void CALLBACK MyTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	CCommunication() {
		FreeState = new CFreeState(this);
		AlarmState = new CAlarmState(this);
		CameraTalkState = new CCameraTalkState(this);
		HostTalkState = new CHostTalkState(this);
		WaitReplyState = new CWaitReplyState(this);

		mState = FreeState;
	};
	~CCommunication() {};
	void HostTalk(CCamera *pDev);
	void CameraTalk();
	void StopTalk();
	void Alarm(CCamera *pDev);
	void StopAlarm();
	void SetVolume(CCamera *pDev, int Volume);
	void ControlLED(int Switch);
	void SetLED(int isON);
	void Handle();
	void GetPortMac(int port);
	void CameraCanTalk(CCamera *pDev);

	void ReplyHostTalk(CCamera *pDev);
	void ReplyCameraTalk(CCamera *pDev);
	void ReplyStopTalk();
	void ReplyAlarm(CCamera *pDev);
	void ReplyStopAlarm(CCamera *pDev);
	void ReplySetVolume(CCamera *pDev);
	void ReplyControlLED(bool isSucceed);
	void ReplySetLED(bool isSucceed);
	void ReplyHandle(bool isSucceed);
	void ReplyGetPortMac(int port);
	void ReplyCameraAskTalk(CCamera *pDev);

	IState* GetFreeState();
	IState* GetAlarmState();
	IState* GetCameraTalkState();
	IState* GetHostTalkState();
	IState* GetWaitReplyState();
	IState* GetState();
	IState* GetOldState();

	void SetFreeState();
	void SetAlarmState();
	void SetCameraTalkState();
	void SetHostTalkState();
	void SetWaitReplyState();

	void RecoveState();
};