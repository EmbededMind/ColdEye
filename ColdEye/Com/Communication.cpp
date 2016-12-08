#include "stdafx.h"
#include "Communication.h"
#include "ColdEyeDlg.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"

UINT CCommunication::CommunicationThread(LPVOID pParam)
{
	CCommunication* pThis = (CCommunication*)pParam;
	pThis->mIsThreadAlive = true;
	DWORD Event = 0;
	for (;;)
	{
		Event = WaitForMultipleObjects(12, pThis->mEventArray, FALSE, INFINITE);
		switch (Event)
		{
		case END_EVENT_NUM:
			pThis->mIsThreadAlive = FALSE;
			ResetEvent(pThis->mEndEvent);
			AfxEndThread(100);
			break;
		case CAMERACANTALK_EVENT_NUM:
			pThis->mState->ReplyCameraAskTalk(pThis->mCurrentpDev[CAMERACANTALK_EVENT_NUM]);
			ResetEvent(pThis->mReplyCameraAskTalkEvent);
			break;
		case STOPTALK_EVENT_NUM:
			ResetEvent(pThis->mReplyStopTalkEvent);
			pThis->mState->StopTalk();
			Event = WaitForSingleObject(pThis->mReplyStopTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyStopTalk();
				ResetEvent(pThis->mReplyStopTalkEvent);
			}
			else
			{
				Print("StopTalk��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mStopTalkEvent);
			break;
		case STOPALARM_EVENT_NUM:
			ResetEvent(pThis->mReplyStopAlarmEvent);
			pThis->mState->StopAlarm(pThis->mCurrentpDev[STOPALARM_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyStopAlarmEvent, TIMEOUT_TIME);
			if (Event = WAIT_OBJECT_0)
			{
				pThis->mState->ReplyStopAlarm(pThis->mCurrentpDev[STOPALARM_EVENT_NUM]);
				ResetEvent(pThis->mReplyStopAlarmEvent);
			}
			else
			{
				Print("StopAlarm��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mStopAlarmEvent);
			break;
		case HOSTTALK_EVENT_NUM:
			ResetEvent(pThis->mReplyHostTalkEvent);
			pThis->mState->HostTalk(pThis->mCurrentpDev[HOSTTALK_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyHostTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyHostTalk(pThis->mCurrentpDev[HOSTTALK_EVENT_NUM]);
				ResetEvent(pThis->mReplyHostTalkEvent);
			}
			else
			{
				Print("HostTalk��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mHostTalkEvent);
			break;
		case CAMERATALK_EVENT_NUM:
			ResetEvent(pThis->mReplyCameraTalkEvent);
			pThis->mState->CameraTalk(pThis->mCurrentpDev[CAMERATALK_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyCameraTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyCameraTalk(pThis->mCurrentpDev[CAMERATALK_EVENT_NUM]);
				ResetEvent(pThis->mReplyCameraTalkEvent);
			}
			else
			{
				Print("CamreaTalk��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mCameraTalkEvent);
			break;
		case ALARM_EVENT_NUM:
			ResetEvent(pThis->mReplyAlarmEvent);
			pThis->mState->Alarm(pThis->mCurrentpDev[ALARM_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyAlarmEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyAlarm(pThis->mCurrentpDev[ALARM_EVENT_NUM]);
				ResetEvent(pThis->mReplyAlarmEvent);
			}
			else
			{
				Print("Alarm��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mAlarmEvent);
			break;
		case CONTROLLED_EVENT_NUM:
			ResetEvent(pThis->mReplyControlLEDEvent);
			pThis->mState->ControlLED(pThis->mCurrentInt[CONTROLLED_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyControlLEDEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyControlLED(pThis->mCurrentBool[CONTROLLED_EVENT_NUM]);
				ResetEvent(pThis->mReplyControlLEDEvent);
			}
			else
			{
				Print("ControlLED��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mControlLEvent);
			break;
		case SETVOLUME_EVENT_NUM:
			ResetEvent(pThis->mReplySetVolumeEvent);
			pThis->mState->SetVolume(pThis->mCurrentpDev[SETVOLUME_EVENT_NUM], pThis->mCurrentInt[SETVOLUME_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplySetVolumeEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplySetVolume(pThis->mCurrentpDev[SETVOLUME_EVENT_NUM], pThis->mCurrentInt[SETVOLUME_EVENT_NUM]);
				ResetEvent(pThis->mReplySetVolumeEvent);
			}
			else
			{
				Print("SetVolume��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mSetVolumeEvent);
			break;
		case SETLED_EVENT_NUM:
			ResetEvent(pThis->mReplySetLEDEvent);
			pThis->mState->SetLED(pThis->mCurrentInt[SETLED_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplySetLEDEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplySetLED(pThis->mCurrentBool[SETLED_EVENT_NUM]);
				ResetEvent(pThis->mReplySetLEDEvent);
			}
			else
			{
				Print("SetLED��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mSetLEDEvent);
			break;
		case HANDLE_EVENT_NUM:
			ResetEvent(pThis->mReplyHandleEvent);
			pThis->mState->Handle();
			Event = WaitForSingleObject(pThis->mReplyHandleEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				Print("HANDLEδ��ʱ");
				pThis->mState->ReplyHandle(pThis->mCurrentBool);
				ResetEvent(pThis->mReplyHandleEvent);
			}
			else
			{
				Print("HANDLE��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mHandleEvent);
			break;
		case GETPORTMAC_EVENT_NUM:
			ResetEvent(pThis->mReplyGetPortMacEvent);
			pThis->mState->GetPortMac(pThis->mCurrentInt[GETPORTMAC_EVENT_NUM]);
			Event = WaitForSingleObject(pThis->mReplyGetPortMacEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyGetPortMac(pThis->mCurrentInt[GETPORTMAC_EVENT_NUM]);
				ResetEvent(pThis->mReplyGetPortMacEvent);
			}
			else
			{
				Print("GETPORTMAC��ʱ��");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mGetPortMacEvent);
			break;
		}
	}
	return 0;
}

BOOL CCommunication::Init(CWnd * pOwner)
{
	ASSERT(pOwner != NULL);
	mpOwner = pOwner;
	mHOwner = static_cast<CColdEyeDlg*>(pOwner)->GetMyMenu().GetHWND();

	if (mIsThreadAlive)
	{
		do
		{
			SetEvent(mEndEvent);
		}while(mIsThreadAlive);
	}

	if (mEndEvent != NULL)
		ResetEvent(mEndEvent);
	else
		mEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyCameraAskTalkEvent != NULL)
		ResetEvent(mReplyCameraAskTalkEvent);
	else
		mReplyCameraAskTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mStopTalkEvent != NULL)
		ResetEvent(mStopTalkEvent);
	else
		mStopTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mStopAlarmEvent != NULL)
		ResetEvent(mStopAlarmEvent);
	else
		mStopAlarmEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mHostTalkEvent != NULL)
		ResetEvent(mHostTalkEvent);
	else
		mHostTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mCameraTalkEvent != NULL)
		ResetEvent(mCameraTalkEvent);
	else
		mCameraTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mAlarmEvent != NULL)
		ResetEvent(mAlarmEvent);
	else
		mAlarmEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mControlLEvent != NULL)
		ResetEvent(mControlLEvent);
	else
		mControlLEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mSetVolumeEvent != NULL)
		ResetEvent(mSetVolumeEvent);
	else
		mSetVolumeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mSetLEDEvent != NULL)
		ResetEvent(mSetLEDEvent);
	else
		mSetLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mHandleEvent != NULL)
		ResetEvent(mHandleEvent);
	else
		mHandleEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mGetPortMacEvent != NULL)
		ResetEvent(mGetPortMacEvent);
	else
		mGetPortMacEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	mEventArray[0] = mEndEvent;
	mEventArray[1] = mReplyCameraAskTalkEvent;
	mEventArray[2] = mStopTalkEvent;
	mEventArray[3] = mStopAlarmEvent;
	mEventArray[4] = mHostTalkEvent;
	mEventArray[5] = mCameraTalkEvent;
	mEventArray[6] = mAlarmEvent;
	mEventArray[7] = mControlLEvent;
	mEventArray[8] = mSetVolumeEvent;
	mEventArray[9] = mSetLEDEvent;
	mEventArray[10] = mHandleEvent;
	mEventArray[11] = mGetPortMacEvent;

	if (mReplyStopTalkEvent != NULL)
		ResetEvent(mReplyStopTalkEvent);
	else
		mReplyStopTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyStopAlarmEvent != NULL)
		ResetEvent(mReplyStopAlarmEvent);
	else
		mReplyStopAlarmEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyHostTalkEvent != NULL)
		ResetEvent(mReplyHostTalkEvent);
	else
		mReplyHostTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyCameraTalkEvent != NULL)
		ResetEvent(mReplyCameraTalkEvent);
	else
		mReplyCameraTalkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyAlarmEvent != NULL)
		ResetEvent(mReplyAlarmEvent);
	else
		mReplyAlarmEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyControlLEDEvent != NULL)
		ResetEvent(mReplyControlLEDEvent);
	else
		mReplyControlLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplySetVolumeEvent != NULL)
		ResetEvent(mReplySetVolumeEvent);
	else
		mReplySetVolumeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplySetLEDEvent != NULL)
		ResetEvent(mReplySetLEDEvent);
	else
		mReplySetLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyHandleEvent != NULL)
		ResetEvent(mReplyHandleEvent);
	else
		mReplyHandleEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyGetPortMacEvent != NULL)
		ResetEvent(mReplyGetPortMacEvent);
	else
		mReplyGetPortMacEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	return 0;
}

BOOL CCommunication::StartThread()
{
	if (!(mThread = AfxBeginThread(CommunicationThread, this)))
		return FALSE;
	return TRUE;
}

void CCommunication::HostTalk(CCamera *pDev)
{
	this->mCurrentpDev[HOSTTALK_EVENT_NUM] = pDev;
	SetEvent(mHostTalkEvent);
}

void CCommunication::CameraTalk()
{
	mCurrentpDev[CAMERATALK_EVENT_NUM] = mPdev;
	SetEvent(mCameraTalkEvent);
}

void CCommunication::StopTalk()
{
	SetEvent(mStopTalkEvent);
}

void CCommunication::Alarm(CCamera *pDev)
{
	this->mCurrentpDev[ALARM_EVENT_NUM] = pDev;
	SetEvent(mAlarmEvent);
}

void CCommunication::StopAlarm(CCamera *pDev)
{
	this->mCurrentpDev[STOPALARM_EVENT_NUM] = pDev;
	SetEvent(mStopAlarmEvent);
}

void CCommunication::SetVolume(CCamera *pDev, int Volume)
{
	this->mCurrentpDev[SETVOLUME_EVENT_NUM] = pDev;
	this->mCurrentInt[SETVOLUME_EVENT_NUM] = Volume;
	SetEvent(mSetVolumeEvent);
}

void CCommunication::ControlLED(int Switch)
{
	this->mCurrentInt[CONTROLLED_EVENT_NUM] = Switch;
	SetEvent(mControlLEvent);
}

void CCommunication::SetLED(int isON)
{
	this->mCurrentInt[SETLED_EVENT_NUM] = isON;
	SetEvent(mSetLEDEvent);
}

void CCommunication::Handle()
{
	Print("HANDLE");
	SetEvent(mHandleEvent);
}

void CCommunication::GetPortMac(int port)
{
	this->mCurrentInt[GETPORTMAC_EVENT_NUM] = port;
	SetEvent(mGetPortMacEvent);
}

void CCommunication::CameraCanTalk(CCamera * pDev)
{
	mState->CameraCanTalk(pDev);
}

void CCommunication::ReplyHostTalk(CCamera *pDev)
{
	this->mCurrentpDev[HOSTTALK_EVENT_NUM] = pDev;
	SetEvent(mReplyHostTalkEvent);
}

void CCommunication::ReplyCameraTalk(CCamera *pDev)
{
	this->mCurrentpDev[CAMERATALK_EVENT_NUM] = pDev;
	SetEvent(mReplyCameraTalkEvent);
}

void CCommunication::ReplyStopTalk()
{
	SetEvent(mReplyStopTalkEvent);
}

void CCommunication::ReplyAlarm(CCamera *pDev)
{
	this->mCurrentpDev[ALARM_EVENT_NUM] = pDev;
	SetEvent(mReplyAlarmEvent);
}

void CCommunication::ReplyStopAlarm(CCamera *pDev)
{
	this->mCurrentpDev[STOPALARM_EVENT_NUM] = pDev;
	SetEvent(mReplyStopAlarmEvent);
}

void CCommunication::ReplySetVolume(CCamera *pDev, int Volume)
{
	this->mCurrentpDev[SETVOLUME_EVENT_NUM] = pDev;
	this->mCurrentInt[SETVOLUME_EVENT_NUM] = Volume;
	SetEvent(mReplySetVolumeEvent);
}

void CCommunication::ReplyControlLED(bool isSucceed)
{
	this->mCurrentBool[CONTROLLED_EVENT_NUM] = isSucceed;
	SetEvent(mReplyControlLEDEvent);
}

void CCommunication::ReplySetLED(bool isSucceed)
{
	this->mCurrentBool[SETLED_EVENT_NUM] = isSucceed;
	SetEvent(mReplySetLEDEvent);
}

void CCommunication::ReplyHandle(bool isSucceed)
{
	Print("ReplyHandle");
	this->mCurrentBool[HANDLE_EVENT_NUM] = isSucceed;
	SetEvent(mReplyHandleEvent);
}

void CCommunication::ReplyGetPortMac(int port)
{
	this->mCurrentInt[GETPORTMAC_EVENT_NUM] = port;
	SetEvent(mReplyGetPortMacEvent);
}

void CCommunication::ReplyCameraAskTalk(CCamera * pDev)
{
	this->mCurrentpDev[CAMERACANTALK_EVENT_NUM] = pDev;
	SetEvent(mReplyCameraAskTalkEvent);
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
	Print("ת������״̬");
	mOldState = mState;
	mState = FreeState;
}

void CCommunication::SetAlarmState()
{
	Print("ת������״̬");
	mOldState = mState;
	mState = AlarmState;
}

void CCommunication::SetCameraTalkState()
{
	Print("ת������ͷ����״̬");
	mOldState = mState;
	mState = CameraTalkState;
}

void CCommunication::SetHostTalkState()
{
	Print("ת����������״̬");
	mOldState = mState;
	mState = HostTalkState;
}

void CCommunication::SetWaitReplyState()
{
	Print("ת���ȴ�״̬");
	mOldState = mState;
	mState = WaitReplyState;
}

void CCommunication::RecoveState()
{
	Print("����ԭ��״̬");
	if (mOldState == WaitReplyState)
		return;
	IState* tmp;
	tmp = mState;
	mState = mOldState;
	mOldState = tmp;
}
