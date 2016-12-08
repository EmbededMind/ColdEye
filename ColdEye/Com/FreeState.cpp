#include "stdafx.h"
#include "FreeState.h"
#include "iostream"
#include "Communication.h"
#include "com\SerialPort.h"
#include "Com\Util.h"
#include "Com\SerialPort.h"

void CFreeState::HostTalk(CCamera *pDev)
{
	Print("主机要讲话...");
	SetEvent(ComManagement->mHostTalkEvent);
}

void CFreeState::Alarm(CCamera *pDev)
{
	Print("要报警");
	SetEvent(ComManagement->mAlarmEvent);
}

void CFreeState::SetVolume(CCamera *pDev, int Volume)
{
	Print("设置音量");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CFreeState::ControlLED(int Switch)
{
	Print("控制LED");
	SetEvent(ComManagement->mControlLEvent);
}

void CFreeState::SetLED(int isON)
{
	Print("设置LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CFreeState::Handle()
{
	Print("握手");
	SetEvent(ComManagement->mHandleEvent);
}

void CFreeState::GetPortMac(int port)
{
	Print("获取MAC地址");
	SetEvent(ComManagement->mGetPortMacEvent);
}

void CFreeState::CameraCanTalk(CCamera * pDev)
{
}

void CFreeState::ReplyCameraAskTalk(CCamera * pDev)
{
	SetEvent(ComManagement->mReplyCameraAskTalkEvent);
}


