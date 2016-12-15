#pragma once
#include "IStatus.h"
#include "FreeState.h"
#include "AlarmState.h"
#include "CameraTalkState.h"
#include "HostTalkState.h"
#include "WaitReply.h"
#include "Device\Camera.h"

#ifndef EVENT_NUM

#define EVENT_NUM
#define END_EVENT_NUM              0
#define CAMERACANTALK_EVENT_NUM    1
#define STOPTALK_EVENT_NUM         2
#define STOPALARM_EVENT_NUM        3
#define HOSTTALK_EVENT_NUM         4
#define CAMERATALK_EVENT_NUM       5
#define ALARM_EVENT_NUM            6
#define TURNOFFLED_EVENT_NUM       7
#define TURNONLED_EVENT_NUM        8
#define SETVOLUME_EVENT_NUM        9
#define SETLED_EVENT_NUM           10
#define HANDLE_EVENT_NUM           11
#define GETPORTMAC_EVENT_NUM       12

#define TIMER_ALARM_STOP           13
#define TIMER_LED_STOP             14

#define TIMEOUT_TIME               500

#endif // !EVENT_NUM


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

	CWinThread* mThread;
	CWnd* mpOwner;
	HWND mHOwner;
	uint8_t mOrder[17];

	HANDLE mEventArray[15];

	HANDLE mReplyStopTalkEvent;
	HANDLE mReplyStopAlarmEvent;
	HANDLE mReplyHostTalkEvent;
	HANDLE mReplyCameraTalkEvent;
	HANDLE mReplyAlarmEvent;
	HANDLE mReplyTurnOnLEDEvent;
	HANDLE mReplyTurnOffLEDEvent;
	HANDLE mReplySetVolumeEvent;
	HANDLE mReplySetLEDEvent;
	HANDLE mReplyHandleEvent;
	HANDLE mReplyGetPortMacEvent;

	bool mIsThreadAlive;

	static UINT CommunicationThread(LPVOID pParam);
	bool mCurrentBool[15];
	CCamera* mCurrentpDev[15];
	int mCurrentInt[15];

	HANDLE mLEDTimer;
	HANDLE mAlarmSoundTimer;

	LARGE_INTEGER liDueTimeAlarmSound;
	LARGE_INTEGER liDueTimeLED;
public:
	HANDLE mStopAlarmEvent;
	HANDLE mEndEvent;
	HANDLE mReplyCameraAskTalkEvent;
	HANDLE mStopTalkEvent;
	HANDLE mHostTalkEvent;
	HANDLE mCameraTalkEvent;
	HANDLE mAlarmEvent;
	HANDLE mTurnOffLEDEvent;
	HANDLE mTurnOnLEDEvent;
	HANDLE mSetVolumeEvent;
	HANDLE mSetLEDEvent;
	HANDLE mHandleEvent;
	HANDLE mGetPortMacEvent;

	CCamera *mPdev;
	LONG mHandle;

	BOOL Init(CWnd* pOwner);
	BOOL StartThread();
	static CCommunication* GetInstance()
	{
		static CCommunication instance;
		return &instance;
	}
	CCommunication() {
		FreeState = new CFreeState(this);
		AlarmState = new CAlarmState(this);
		CameraTalkState = new CCameraTalkState(this);
		HostTalkState = new CHostTalkState(this);
		WaitReplyState = new CWaitReplyState(this);

		mState = FreeState;
	};
	~CCommunication() ;
	void HostTalk(CCamera *pDev);
	void CameraTalk();
	void StopTalk();
	void Alarm(CCamera *pDev);
	void StopAlarm(CCamera *pDev);
	void SetVolume(CCamera *pDev, int Volume);
	void TurnOnLED();
	void TurnOffLED();
	void SetLED(int isON);
	void Handle();
	void GetPortMac(int port);
	//void CameraCanTalk(CCamera *pDev);

	void ReplyHostTalk(CCamera *pDev);
	void ReplyCameraTalk(CCamera *pDev);
	void ReplyStopTalk();
	void ReplyAlarm(CCamera *pDev);
	void ReplyStopAlarm(CCamera *pDev);
	void ReplySetVolume(CCamera *pDev, int Volume);
	void ReplyTurnOnLED();
	void ReplyTurnOffLED();
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