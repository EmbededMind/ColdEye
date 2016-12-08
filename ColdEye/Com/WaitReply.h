#pragma once
#include "IStatus.h"
class CCommunication;
class CWaitReplyState:public IState
{
private:
	uint8_t mOrder[17];
public:
	CWaitReplyState(CCommunication *ComManagement)
	{
		this->ComManagement = ComManagement;
	};
	~CWaitReplyState() {};

	CCommunication *ComManagement;

	//void StopAlarm();

	void ReplyHostTalk(CCamera *pDev);
	void ReplyCameraTalk(CCamera *pDev);
	void ReplyStopTalk();
	void ReplyAlarm(CCamera *pDev);
	void ReplyStopAlarm(CCamera *pDev);
	void ReplySetVolume(bool);
	void ReplyControlLED(bool);
	void ReplySetLED(bool);
	void ReplyHandle();
	void ReplyGetPortMac(int port);
};

