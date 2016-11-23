#include "stdafx.h"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\RecordAlarmSound.h"
#include "Com\RecordAlarmSound.h"
CCommunication::CCommunication()
{
}


CCommunication::~CCommunication()
{
}

bool CCommunication::IsChannelCleaning()
{
	if (this->mPdev || this->mTalkHandle)
		return false;
	return true;
}

bool CCommunication::CleanChannel()
{
	this->mPdev = nullptr;
	this->mTalkHandle = NULL;
	return false;
}

bool CCommunication::AskTalk(CCamera * pDev)
{
	printf("AskTalk\n");
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	return true;
}

bool CCommunication::RecTalkProc(uint8_t *pch)
{
	if (pch[5] == 0x01)
	{
		uint64_t mac64;
		mac64 = CUtil::ArrayToUint64(&pch[6]);
		if (this->mPdev && this->mPdev != Mac_CCamera_Map[mac64])
		{
			H264_DVR_StopVoiceCom(this->mTalkHandle);
			CleanChannel();
		}
		if (this->mPdev == Mac_CCamera_Map[mac64])
		{
			return true;
		}
		if (this->mTalkHandle = H264_DVR_StartLocalVoiceCom(Mac_CCamera_Map[mac64]->GetLoginId()))
		{
			this->mPdev = Mac_CCamera_Map.at(mac64);
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool CCommunication::ReplyTalk(uint8_t *pch)
{
	if (IsChannelCleaning())
	{
		uint64_t mac64;
		mac64 = CUtil::ArrayToUint64(&pch[6]);
		if (this->mTalkHandle = H264_DVR_StartLocalVoiceCom(Mac_CCamera_Map.at(mac64)->GetLoginId()))
		{
			this->mPdev = Mac_CCamera_Map.at(mac64);
			CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x01, this->mPdev);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
			return true;
		}
	}
	else
	{
		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x02, this->mPdev);
		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
		return false;
	}
	return false;
}

bool CCommunication::OverTalk()
{
	if (!IsChannelCleaning())
	{
		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x03, 0x00, this->mPdev);
		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
		return true;
	}
	return false;
}

bool CCommunication::RecOverTalkProc(uint8_t *pch)
{
	if (!IsChannelCleaning())
	{
		if (pch[5] == 1)
		{
			uint64_t mac64;
			mac64 = CUtil::ArrayToUint64(&pch[6]);
			if (Mac_CCamera_Map.at(mac64) == this->mPdev)
			{
				if (H264_DVR_StopVoiceCom(this->mTalkHandle))
				{
					CleanChannel();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CCommunication::YouTalk()
{
	if (!IsChannelCleaning())
	{
		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x02, 0x02, 0x00, this->mPdev);
		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
		return true;
	}
	return false;
}

bool CCommunication::RecYouTalkProc(uint8_t *pch)
{
	if (!IsChannelCleaning())
	{
		if (pch[5] == 1)
		{
			//I don't konwn;
		}
		else
		{
			//I don't konwn;
		}
	}
	return false;
}

bool CCommunication::AskSetVolume(CCamera *pDev, uint8_t Volume)
{
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x01, Volume, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	return true;
}

uint8_t CCommunication::RecSetVolumeProc(uint8_t *pch)
{
	if (pch[5] == 1)
	{
		return pch[4];
	}
	else
	{
		return pch[4];
	}
}

bool CCommunication::Alarm(CCamera * pDev)
{
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	return true;
}

bool CCommunication::RecAlarmProc(uint8_t *pch)
{
	if (pch[5] == 1)
	{
		H264_DVR_StopVoiceCom(this->mTalkHandle);
		CleanChannel();
		uint64_t mac64;
		mac64 = CUtil::ArrayToUint64(&pch[6]);
		CRecordAlarmSound::GetInstance()->Play(Mac_CCamera_Map.at(mac64));
		return true;
	}
	else
	{
		return false;
	}
}

bool CCommunication::OverAlarm(CCamera * pDev)
{
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x01, 0x00, pDev);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	return true;
}

bool CCommunication::RecOverAlarmProc(uint8_t *pch)
{
	if (pch[5] == 1)
	{
		uint64_t mac64;
		mac64 = CUtil::ArrayToUint64(&pch[6]);
		CRecordAlarmSound::GetInstance()->StopTalk();
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool CCommunication::Handle(uint8_t prm)
{
	CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
	CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
	return true;
}

uint8_t CCommunication::RecHandleProc(uint8_t * pch)
{

	return true;
}
