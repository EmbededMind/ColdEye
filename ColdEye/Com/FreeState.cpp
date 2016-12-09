#include "stdafx.h"
#include "FreeState.h"
#include "iostream"
#include "Communication.h"
#include "com\SerialPort.h"
#include "Com\Util.h"
#include "Com\SerialPort.h"

void CFreeState::HostTalk(CCamera *pDev)
{
	Print("����Ҫ����...");
	SetEvent(ComManagement->mHostTalkEvent);
}

void CFreeState::Alarm(CCamera *pDev)
{
	Print("Ҫ����");
	SetEvent(ComManagement->mAlarmEvent);
}

void CFreeState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CFreeState::TurnOffLED()
{
	Print("TurnOffLED");
	SetEvent(ComManagement->mTurnOffLEDEvent);
}

//void CFreeState::ControlLED(int Switch)
//{
//	Print("����LED");
//	SetEvent(ComManagement->mControlLEDEvent);
//}

void CFreeState::SetLED(int isON)
{
	Print("����LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CFreeState::Handle()
{
	Print("����");
	SetEvent(ComManagement->mHandleEvent);
}

void CFreeState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	SetEvent(ComManagement->mGetPortMacEvent);
}

void CFreeState::CameraCanTalk(CCamera * pDev)
{
}

void CFreeState::ReplyCameraAskTalk(CCamera * pDev)
{
	SetEvent(ComManagement->mReplyCameraAskTalkEvent);
}


