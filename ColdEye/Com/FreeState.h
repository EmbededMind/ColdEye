#pragma once
#include "IStatus.h"
#include "Device\Camera.h"

class CCommunication;
class CFreeState :public IState
{
private:
	uint8_t mOrder[17];
public:
	CFreeState(CCommunication *ComManagement) {
		this->ComManagement = ComManagement;
	};
	~CFreeState() {};
	CCommunication *ComManagement;

	void HostTalk(CCamera *pDev);
	void Alarm(CCamera *pDev);
	void SetVolume(CCamera *pDev, int Volume);
	void TurnOffLED();
	void SetLED(int isON);
	void Handle();
	void GetPortMac(int port);
	void CameraCanTalk(CCamera *pDev);

	void ReplyCameraAskTalk(CCamera *pDev);
};