#include "stdafx.h"
#include "Communication.h"

void CCommunication::HostTalk(CCamera *pDev)
{
	mState->HostTalk(pDev);
}

void CCommunication::CameraTalk()
{
	mState->CameraTalk(mPdev);
}

void CCommunication::StopTalk()
{
	mState->StopTalk();
}

void CCommunication::Alarm(CCamera *pDev)
{
	mState->Alarm(pDev);
}

void CCommunication::StopAlarm()
{
	mState->StopAlarm();
}

void CCommunication::SetVolume(CCamera *pDev, int Volume)
{
	mState->SetVolume(pDev, Volume);
}

void CCommunication::ControlLED(int Switch)
{
	mState->ControlLED(Switch);
}

void CCommunication::SetLED(int isON)
{
	mState->SetLED(isON);
}

void CCommunication::Handle()
{
	mState->Handle();
}

void CCommunication::GetPortMac(int port)
{
	mState->GetPortMac(port);
}

void CCommunication::CameraCanTalk(CCamera * pDev)
{
	mState->CameraCanTalk(pDev);
}

void CCommunication::ReplyHostTalk(CCamera *pDev)
{
	mState->ReplyHandle(pDev);
}

void CCommunication::ReplyCameraTalk(CCamera *pDev)
{
	mState->ReplyCameraTalk(pDev);
}

void CCommunication::ReplyStopTalk()
{
	mState->ReplyStopTalk();
}

void CCommunication::ReplyAlarm(CCamera *pDev)
{
	mState->ReplyAlarm(pDev);
}

void CCommunication::ReplyStopAlarm(CCamera *pDev)
{
	mState->ReplyStopAlarm(pDev);
}

void CCommunication::ReplySetVolume(CCamera *pDev)
{
	mState->ReplySetVolume(pDev);
}

void CCommunication::ReplyControlLED(bool isSucceed)
{
	mState->ReplyControlLED(isSucceed);
}

void CCommunication::ReplySetLED(bool isSucceed)
{
	mState->ReplySetLED(isSucceed);
}

void CCommunication::ReplyHandle(bool isSucceed)
{
	mState->ReplyHandle(isSucceed);
}

void CCommunication::ReplyGetPortMac(int port)
{
	mState->ReplyGetPortMac(port);
}

void CCommunication::ReplyCameraAskTalk(CCamera * pDev)
{
	mState->ReplyCameraAskTalk(pDev);
}

IState* CCommunication::GetFreeState()
{
	return FreeState;
}

IState* CCommunication::GetAlarmState()
{
	return AlarmState;
}

IState* CCommunication::GetCameraTalkState()
{
	return CameraTalkState;
}

IState* CCommunication::GetHostTalkState()
{
	return HostTalkState;
}

IState* CCommunication::GetWaitReplyState()
{
	return WaitReplyState;
}

IState * CCommunication::GetState()
{
	return mState;
}

IState * CCommunication::GetOldState()
{
	return mOldState;
}

void CCommunication::SetFreeState()
{
	mOldState = mState;
	mState = FreeState;
}

void CCommunication::SetAlarmState()
{
	mOldState = mState;
	mState = AlarmState;
}

void CCommunication::SetCameraTalkState()
{
	mOldState = mState;
	mState = CameraTalkState;
}

void CCommunication::SetHostTalkState()
{
	mOldState = mState;
	mState = HostTalkState;
}

void CCommunication::SetWaitReplyState()
{
	mOldState = mState;
	mState = WaitReplyState;
}

void CCommunication::RecoveState()
{
	mState = mOldState;
}
