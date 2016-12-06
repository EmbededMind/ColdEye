#include "stdafx.h"
#include "AlarmState.h"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include <iostream>

void CAlarmState::HostTalk(CCamera *pDev)
{
	Print("������...ȡ������...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CAlarmState::StopTalk()
{
	Print("������...ȡ������...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetFreeState();
}

void CAlarmState::StopAlarm()
{
	Print("������...ȡ������...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetFreeState();
}

//void CAlarmState::SetVolume(CCamera *pDev, int Volume)
//{
//	Print("��������");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}

void CAlarmState::ControlLED(int Switch)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

//void CAlarmState::SetLED(int isON)
//{
//	Print("����LED");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, isON, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}
//
//void CAlarmState::Handle()
//{
//	Print("����");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}
//
//void CAlarmState::GetPortMac(int port)
//{
//	Print("��ȡMAC��ַ");
//	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, 0x00, NULL);
//	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	ComManagement->SetWaitReplyState();
//}

void CAlarmState::ReplyCameraAskTalk(CCamera * pDev)
{
	Print("������...ȡ������...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}



