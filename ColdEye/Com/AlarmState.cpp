#include "stdafx.h"
#include "AlarmState.h"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include <iostream>

void CAlarmState::HostTalk(CCamera *pDev)
{
	Print("������...ȡ������...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::StopTalk()
{
	Print("������...ȡ������...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::StopAlarm(CCamera *pDev)
{
	Print("������...ȡ������...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CAlarmState::TurnOnLED()
{
	Print("TurnOnLED");
	SetEvent(ComManagement->mTurnOnLEDEvent);
}

void CAlarmState::TurnOffLED()
{
	Print("TurnOffLED");
	SetEvent(ComManagement->mTurnOffLEDEvent);
}

void CAlarmState::SetLED(int isON)
{
	Print("����LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CAlarmState::Handle()
{
	Print("����");
	SetEvent(ComManagement->mHandleEvent);
}

void CAlarmState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	SetEvent(ComManagement->mGetPortMacEvent);
}

void CAlarmState::ReplyCameraAskTalk(CCamera * pDev)
{
	Print("������...ȡ������...");
	SetEvent(ComManagement->mStopAlarmEvent);
}



