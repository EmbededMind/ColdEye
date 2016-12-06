#pragma once
#include "Device\Camera.h"
class IState {
public: 
	IState() {};
	~IState() {};
	virtual void HostTalk(CCamera *pDev) {};
	virtual void CameraTalk(CCamera *pDev) {};
	virtual void StopTalk() {};
	virtual void Alarm(CCamera *pDev) {};
	virtual void StopAlarm() {};
	virtual void SetVolume(CCamera *pDev, int Volume) {};
	virtual void ControlLED(int Switch) {};
	virtual void SetLED(int isON) {};
	virtual void Handle() {};
	virtual void GetPortMac(int port) {};
	virtual void CameraCanTalk(CCamera *pDev) {};


	virtual void ReplyHostTalk(CCamera *) {};
	virtual void ReplyCameraTalk(CCamera *) {};
	virtual void ReplyStopTalk() {};
	virtual void ReplyAlarm(CCamera *) {};
	virtual void ReplyStopAlarm(CCamera *) {};
	virtual void ReplySetVolume(bool) {};
	virtual void ReplyControlLED(bool) {};
	virtual void ReplySetLED(bool) {};
	virtual void ReplyHandle(bool) {};
	virtual void ReplyGetPortMac(int port) {};
	virtual void ReplyCameraAskTalk(CCamera *pDev) {};
};