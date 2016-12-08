#include "stdafx.h"
#include "CameraTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CCameraTalkState::HostTalk(CCamera *pDev)
{
	Print("主机要讲话...");
	SetEvent(ComManagement->mHostTalkEvent);
}

void CCameraTalkState::StopTalk()
{
	Print("停止讲话");
	SetEvent(ComManagement->mStopTalkEvent);
}

void CCameraTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("设置音量");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CCameraTalkState::ControlLED(int Switch)
{
	Print("控制LED");
	SetEvent(ComManagement->mControlLEvent);
}

void CCameraTalkState::SetLED(int isON)
{
	Print("设置LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CCameraTalkState::Handle()
{
	Print("握手");
	SetEvent(ComManagement->mHandleEvent);
}

void CCameraTalkState::GetPortMac(int port)
{
	Print("获取MAC地址");
	SetEvent(ComManagement->mGetPortMacEvent);
}

