#include "stdafx.h"
#include "WaitReply.h"
#include "iostream"
#include "Communication.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include "Com\RecordAlarmSound.h"
#include "ColdEye.h"

//void CWaitReplyState::StopAlarm()
//{
//	if (ComManagement->GetOldState() == ComManagement->GetAlarmState())
//	{
//		CUtil::LoadOrder(mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, ComManagement->mPdev);
//		CSerialPort::GetInstance(COM_CAM)->WriteToPort(mOrder, 17);
//	}
//}

void CWaitReplyState::ReplyHostTalk(CCamera *pDev)
{
	Print("ReplyHostTalk");
	if (pDev)//��ָ��
	{
		if (ComManagement->GetOldState() == ComManagement->GetFreeState())//����״̬�������
		{
			Print("����״̬�������");
			LONG handle = H264_DVR_StartLocalVoiceCom(pDev->GetLoginId());
			ComManagement->mHandle = handle;
			ComManagement->mPdev = pDev;
			ComManagement->SetHostTalkState();

			pDev->SetTalking(true);
			::MessageBeep(MB_ICONASTERISK);
			return;
		}
		else if(ComManagement->GetOldState() == ComManagement->GetCameraTalkState())//����ͷ����������
		{
			Print("����ͷ����������");
			if (ComManagement->mPdev == pDev)
			{
				ComManagement->SetHostTalkState();
				return;
			}
			else
			{
				LONG handle;
				H264_DVR_StopVoiceCom(ComManagement->mHandle);
				ComManagement->mPdev->SetTalking(FALSE);
				handle = H264_DVR_StartLocalVoiceCom(pDev->GetLoginId());
				ComManagement->mHandle = handle;
				ComManagement->mPdev = pDev;
				ComManagement->SetHostTalkState();

				pDev->SetTalking(true);
				::MessageBeep(MB_ICONASTERISK);
				return;
			}
		}
	}
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyCameraTalk(CCamera *pDev)
{
	if (pDev)
	{
		ComManagement->SetCameraTalkState();
		return;
	}
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyStopTalk()
{
	if (ComManagement->mPdev)
	{
		H264_DVR_StopVoiceCom(ComManagement->mHandle);

		ComManagement->mPdev->SetTalking(false);

		ComManagement->mHandle = NULL;
		ComManagement->mPdev = NULL;
		ComManagement->SetFreeState();


		return;
	}
	ComManagement->SetFreeState();
}

void CWaitReplyState::ReplyAlarm(CCamera *pDev)
{
	if (pDev)
	{
		uint8_t type = ((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_id;
		CRecordAlarmSound::GetInstance()->Play(pDev, type);
		ComManagement->SetAlarmState();
		return;
	}
	ComManagement->SetFreeState();
}

void CWaitReplyState::ReplyStopAlarm(CCamera *pDev)
{
	if (pDev)
	{
		CRecordAlarmSound::GetInstance()->StopTalk();
		ComManagement->SetFreeState();
		return;
	}
	ComManagement->SetFreeState();
}

void CWaitReplyState::ReplySetVolume(CCamera *pDev, int Volume)
{
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyTurnOnLED()
{
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyTurnOffLED()
{
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplySetLED(bool isSucceed)
{
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyHandle(bool isSucceed)
{
	ComManagement->RecoveState();
}

void CWaitReplyState::ReplyGetPortMac(int port)
{
	ComManagement->RecoveState();
}

