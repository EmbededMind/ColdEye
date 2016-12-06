#pragma once
#include "IStatus.h"
#include "Device\Camera.h"

class CCommunication;
class CCameraTalkState:public IState
{
private:
	uint8_t mOrder[17];
public:
	CCameraTalkState(CCommunication *ComManagement)
	{
		this->ComManagement = ComManagement;
	};
	~CCameraTalkState() {};

	CCommunication *ComManagement;

	void HostTalk(CCamera *pDev);
	void StopTalk();
	void SetVolume(CCamera *pDev, int Volume);
	void ControlLED(int Switch);
	void SetLED(int isON);
	void Handle();
	void GetPortMac(int port);
};

