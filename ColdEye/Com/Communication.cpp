#include "stdafx.h"
#include "Communication.h"

void CCommunication::MyTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	KillTimer(NULL, GetInstance()->mTimerID);
	if(GetInstance()->GetState() == GetInstance()->GetWaitReplyState())
		GetInstance()->RecoveState();
}

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
	mState->ReplyHostTalk(pDev);
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
	Print("转到空闲状态");
	KillTimer(NULL, mTimerID);
	mOldState = mState;
	mState = FreeState;
}

void CCommunication::SetAlarmState()
{
	Print("转到报警状态");
	KillTimer(NULL, mTimerID);
	mOldState = mState;
	mState = AlarmState;
}

void CCommunication::SetCameraTalkState()
{
	Print("转到摄像头讲话状态");
	KillTimer(NULL, mTimerID);
	mOldState = mState;
	mState = CameraTalkState;
}

void CCommunication::SetHostTalkState()
{
	Print("转到主机讲话状态");
	KillTimer(NULL, mTimerID);
	mOldState = mState;
	mState = HostTalkState;
}

void CCommunication::SetWaitReplyState()
{
	Print("转到等待状态");
	mOldState = mState;
	mState = WaitReplyState;
	mTimerID = SetTimer(NULL, NULL, 200, MyTimerProc);
}

void CCommunication::RecoveState()
{
	Print("返回原来状态");
	IState* tmp;
	KillTimer(NULL, mTimerID);
	tmp = mState;
	mState = mOldState;
	mOldState = tmp;
}
