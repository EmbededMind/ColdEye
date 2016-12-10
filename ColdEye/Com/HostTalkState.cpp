#include "stdafx.h"
#include "HostTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CHostTalkState::CameraTalk(CCamera *pDev)
{
	Print("����ͷ����");
	SetEvent(ComManagement->mCameraTalkEvent);
}

void CHostTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	SetEvent(ComManagement->mSetVolumeEvent);
}

void CHostTalkState::TurnOffLED()
{
	Print("TurnOffLED");
	SetEvent(ComManagement->mTurnOffLEDEvent);
}

void CHostTalkState::SetLED(int isON)
{
	Print("����LED");
	SetEvent(ComManagement->mSetLEDEvent);
}

void CHostTalkState::Handle()
{
	Print("����");
	SetEvent(ComManagement->mHandleEvent);
}

void CHostTalkState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	SetEvent(ComManagement->mGetPortMacEvent);
}

