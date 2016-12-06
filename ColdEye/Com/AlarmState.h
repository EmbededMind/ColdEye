#pragma once
#include "IStatus.h"
#include "Device\Camera.h"

class CCommunication;

class CAlarmState:public IState
{
private:
	uint8_t mOrder[17];
public:
	CAlarmState(CCommunication *ComManagement) {
		this->ComManagement = ComManagement;
	};
	~CAlarmState() {};
	CCommunication *ComManagement;

	void HostTalk(CCamera *pDev);
	void StopTalk();
	void StopAlarm();
	//void SetVolume(CCamera *pDev, int Volume);
	void ControlLED(int Switch);
	//void SetLED(int isON);
	//void Handle();
	//void GetPortMac(int port);

	void ReplyCameraAskTalk(CCamera *pDev);
};

