#include "stdafx.h"
#include "CameraTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CCameraTalkState::HostTalk(CCamera *pDev)
{
	Print("����Ҫ����...");
	SetEvent(ComManagement->mHostTalkEvent);
}

void CCameraTalkState::StopTalk()
{
	Print("ֹͣ����");
	SetEvent(ComManagement->mStopTalkEvent);
}

void CCameraTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CCameraTalkState::TurnOffLED()
{
	Print("TurnOffLED");
	SetEvent(ComManagement->mTurnOffLEDEvent);
}

void CCameraTalkState::SetLED(int isON)
{
	Print("����LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CCameraTalkState::Handle()
{
	Print("����");
	SetEvent(ComManagement->mHandleEvent);
}

void CCameraTalkState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	SetEvent(ComManagement->mGetPortMacEvent);
}

