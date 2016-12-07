#include "stdafx.h"
#include "HostTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CHostTalkState::CameraTalk(CCamera *pDev)
{
	Print("����ͷ����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x02, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CHostTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CHostTalkState::ControlLED(int Switch)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CHostTalkState::SetLED(int isON)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, 0x01, isON, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CHostTalkState::Handle()
{
	Print("����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CHostTalkState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

