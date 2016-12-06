#include "stdafx.h"
#include "CameraTalkState.h"
#include <iostream>
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

void CCameraTalkState::HostTalk(CCamera *pDev)
{
	Print("主机要讲话...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::StopTalk()
{
	Print("停止讲话");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x03, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::SetVolume(CCamera *pDev, int Volume)
{
	Print("设置音量");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::ControlLED(int Switch)
{
	Print("控制LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::SetLED(int isON)
{
	Print("设置LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, isON, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::Handle()
{
	Print("握手");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CCameraTalkState::GetPortMac(int port)
{
	Print("获取MAC地址");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

