#include "stdafx.h"
#include "CameraTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CCameraTalkState::HostTalk(CCamera *pDev)
{
	Print("����Ҫ����...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::StopTalk()
{
	Print("ֹͣ����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x03, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::ControlLED(int Switch)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::SetLED(int isON)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, isON, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::Handle()
{
	Print("����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

