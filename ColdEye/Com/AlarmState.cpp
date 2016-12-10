#include "stdafx.h"
#include "AlarmState.h"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include <iostream>

void CAlarmState::HostTalk(CCamera *pDev)
{
	Print("报警中...取消报警...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::StopTalk()
{
	Print("报警中...取消报警...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::StopAlarm(CCamera *pDev)
{
	Print("报警中...取消报警...");
	SetEvent(ComManagement->mStopAlarmEvent);
}

void CAlarmState::SetVolume(CCamera *pDev, int Volume)
{
	Print("设置音量");
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
	Print("设置LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CAlarmState::Handle()
{
	Print("握手");
	SetEvent(ComManagement->mHandleEvent);
}

void CAlarmState::GetPortMac(int port)
{
	Print("获取MAC地址");
	SetEvent(ComManagement->mGetPortMacEvent);
}

void CAlarmState::ReplyCameraAskTalk(CCamera * pDev)
{
	Print("报警中...取消报警...");
	SetEvent(ComManagement->mStopAlarmEvent);
}



