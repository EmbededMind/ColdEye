#include "stdafx.h"
#include "AlarmState.h"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include <iostream>

void CAlarmState::HostTalk(CCamera *pDev)
{
	Print("报警中...取消报警...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CAlarmState::StopTalk()
{
	Print("报警中...取消报警...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CAlarmState::StopAlarm(CCamera *pDev)
{
	Print("报警中...取消报警...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

//void CAlarmState::SetVolume(CCamera *pDev, int Volume)
//{
//	Print("设置音量");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}

void CAlarmState::ControlLED(int Switch)
{
	Print("控制LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

//void CAlarmState::SetLED(int isON)
//{
//	Print("设置LED");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, isON, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}
//
//void CAlarmState::Handle()
//{
//	Print("握手");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}
//
//void CAlarmState::GetPortMac(int port)
//{
//	Print("获取MAC地址");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}

void CAlarmState::ReplyCameraAskTalk(CCamera * pDev)
{
	Print("报警中...取消报警...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}



