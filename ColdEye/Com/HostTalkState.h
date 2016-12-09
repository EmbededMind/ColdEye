#pragma once
#include "IStatus.h"
class CCommunication;
class CHostTalkState:public IState
{
private:
	uint8_t mOrder[17];
public:
	CHostTalkState(CCommunication *ComManagement)
	{
		this->ComManagement = ComManagement;
	};
	~CHostTalkState() {};
	CCommunication *ComManagement;

	void CameraTalk(CCamera *pDev);
	void SetVolume(CCamera *pDev, int Volume);
	void TurnOffLED();
	void SetLED(int isON);
	void Handle();
	void GetPortMac(int port);

};

