#pragma once
#include "Device\Camera.h"
#include "Com\Util.h"
class CCommunication
{
public:
	~CCommunication();
	static CCommunication* GetInstance()
	{
		static CCommunication com;
		return &com;
	};
private:
	CCommunication();
	uint8_t mOrder[17];
	bool mIsAlarm;
public:
	CCamera * mPdev;
	long mTalkHandle;
	bool IsChannelCleaning();
	bool CleanChannel();

	bool AskTalk(CCamera *pDev);
	bool RecTalkProc(uint8_t *pch);

	bool ReplyTalk(uint8_t *pch);

	bool OverTalk();
	bool RecOverTalkProc(uint8_t *pch);

	bool YouTalk();
	bool RecYouTalkProc(uint8_t *pch);

	bool AskSetVolume(CCamera *pDev, uint8_t Volume);
	uint8_t RecSetVolumeProc(uint8_t *pch);

	bool Alarm(CCamera *pDev);
	bool RecAlarmProc(uint8_t *pch);

	bool OverAlarm(CCamera *pDev);
	bool RecOverAlarmProc(uint8_t *pch);

	bool Handle(uint8_t param);
	bool Handle(uint8_t param, uint8_t port);
	bool RecHandleProc(uint8_t *pch);

	bool ControlLED(CCamera *pDev, uint8_t Switch);
};

