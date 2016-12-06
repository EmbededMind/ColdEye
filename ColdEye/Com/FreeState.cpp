#include "stdafx.h"
#include "FreeState.h"
#include "iostream"
#include "Communication.h"
#include "com\SerialPort.h"
#include "Com\Util.h"
#include "Com\SerialPort.h"

void CFreeState::HostTalk(CCamera *pDev)
{
	Print("����Ҫ����...");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::Alarm(CCamera *pDev)
{
	Print("Ҫ����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->mPdev = pDev;
	ComManagement->SetWaitReplyState();
}

void CFreeState::SetVolume(CCamera *pDev, int Volume)
{
	Print("��������");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::ControlLED(int Switch)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x06, Switch, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::SetLED(int isON)
{
	Print("����LED");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x07, isON, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::Handle()
{
	Print("����");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::GetPortMac(int port)
{
	Print("��ȡMAC��ַ");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, port, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	ComManagement->SetWaitReplyState();
}

void CFreeState::CameraCanTalk(CCamera * pDev)
{
	if (pDev)
	{
		Print("��������ͷ����");
		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x01, pDev);
		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
		ComManagement->SetCameraTalkState();
	}
	else
	{
		Print("����ͷ�޷�����");
		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x02, NULL);
		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
		ComManagement->SetFreeState();
	}
}

void CFreeState::ReplyCameraAskTalk(CCamera * pDev)
{
	LONG handle = H264_DVR_StartLocalVoiceCom(pDev->GetLoginId());
	if (handle)
	{
		ComManagement->mHandle = handle;
		CameraCanTalk(pDev);
	}
	else
	{
		CameraCanTalk(NULL);
	}
}


