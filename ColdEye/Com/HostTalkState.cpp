#include "stdafx.h"
#include "HostTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CHostTalkState::CameraTalk(CCamera *pDev)
{
	Print("摄像头讲话");
	SetEvent(ComManagement->mCameraTalkEvent);
}

void CHostTalkState::StopTalk()
{
	Print("停止讲话");
	SetEvent(ComManagement->mStopTalkEvent);
}

void CHostTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("设置音量");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CHostTalkState::TurnOffLED()
{
	Print("TurnOffLED");
	SetEvent(ComManagement->mTurnOffLEDEvent);
}

void CHostTalkState::SetLED(int isON)
{
	Print("设置LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CHostTalkState::Handle()
{
	Print("握手");
	SetEvent(ComManagement->mHandleEvent);
}

void CHostTalkState::GetPortMac(int port)
{
	Print("获取MAC地址");
	SetEvent(ComManagement->mGetPortMacEvent);
}

