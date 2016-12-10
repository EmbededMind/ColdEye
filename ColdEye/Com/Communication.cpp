#include "stdafx.h"
#include "Communication.h"
#include "ColdEyeDlg.h"
#include "ColdEye.h"
#include "Com\SerialPort.h"
#include "Com\Util.h"
#include "Com\RecordAlarmSound.h"

static SYSTEMTIME st;     //SYSTEMTIME结构，用来设置第1次通知 的时间
static FILETIME ftLocal, ftUTC; //FILETIME结构，用来接受STSTEMTIME结构的转换


UINT CCommunication::CommunicationThread(LPVOID pParam)
{
	CCommunication* pThis = (CCommunication*)pParam;
	pThis->mIsThreadAlive = true;
	DWORD Event = 0;
	for (;;)
	{
		Event = WaitForMultipleObjects(15, pThis->mEventArray, FALSE, INFINITE);
		switch (Event)
		{
		case END_EVENT_NUM:
			pThis->mIsThreadAlive = FALSE;
			ResetEvent(pThis->mEndEvent);
			AfxEndThread(100);
			break;
		case CAMERACANTALK_EVENT_NUM:
		{
			LONG handle = H264_DVR_StartLocalVoiceCom(pThis->mCurrentpDev[CAMERACANTALK_EVENT_NUM]->GetLoginId());
			if (handle)
			{
				pThis->mHandle = handle;
				CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x01, pThis->mCurrentpDev[CAMERACANTALK_EVENT_NUM]);
				CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
				pThis->SetCameraTalkState();
			}
			else
			{
				CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x03, 0x00, 0x02, NULL);
				CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
				pThis->SetFreeState();
			}
			ResetEvent(pThis->mReplyCameraAskTalkEvent);
			break;
		}
		case STOPTALK_EVENT_NUM:
			ResetEvent(pThis->mReplyStopTalkEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x02, 0x03, 0x00, pThis->mPdev);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyStopTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyStopTalk();
				ResetEvent(pThis->mReplyStopTalkEvent);
			}
			else
			{
				Print("StopTalk超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mStopTalkEvent);
			break;
		case STOPALARM_EVENT_NUM:
			ResetEvent(pThis->mReplyStopAlarmEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x04, 0x02, 0x00, pThis->mPdev);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyStopAlarmEvent, 100000);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyStopAlarm(pThis->mCurrentpDev[STOPALARM_EVENT_NUM]);
				ResetEvent(pThis->mReplyStopAlarmEvent);
			}
			else
			{
				Print("Event %ld", Event);
				Print("StopAlarm超时了");
				long err = GetLastError();
				Print("err = %d", err);
				pThis->RecoveState();
			}
			ResetEvent(pThis->mStopAlarmEvent);
			break;
		case HOSTTALK_EVENT_NUM:
			ResetEvent(pThis->mReplyHostTalkEvent);
			pThis->mState->HostTalk(pThis->mCurrentpDev[HOSTTALK_EVENT_NUM]);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x02, 0x01, 0x00, pThis->mCurrentpDev[HOSTTALK_EVENT_NUM]);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyHostTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyHostTalk(pThis->mCurrentpDev[HOSTTALK_EVENT_NUM]);
				ResetEvent(pThis->mReplyHostTalkEvent);
			}
			else
			{
				Print("HostTalk超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mHostTalkEvent);
			break;
		case CAMERATALK_EVENT_NUM:
			ResetEvent(pThis->mReplyCameraTalkEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x02, 0x02, 0x00, pThis->mCurrentpDev[CAMERATALK_EVENT_NUM]);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyCameraTalkEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyCameraTalk(pThis->mCurrentpDev[CAMERATALK_EVENT_NUM]);
				ResetEvent(pThis->mReplyCameraTalkEvent);
			}
			else
			{
				Print("CamreaTalk超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mCameraTalkEvent);
			break;
		case ALARM_EVENT_NUM:
			ResetEvent(pThis->mReplyAlarmEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x04, 0x01, 0x00, pThis->mCurrentpDev[ALARM_EVENT_NUM]);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->mPdev = pThis->mCurrentpDev[ALARM_EVENT_NUM];
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyAlarmEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyAlarm(pThis->mCurrentpDev[ALARM_EVENT_NUM]);
				ResetEvent(pThis->mReplyAlarmEvent);
			}
			else
			{
				Print("Alarm超时了");
				pThis->RecoveState();
			}
			pThis->TurnOnLED();
			CancelWaitableTimer(pThis->mAlarmSoundTimer);
			pThis->liDueTimeAlarmSound.LowPart = 0;
			pThis->liDueTimeAlarmSound.HighPart = 0;
			pThis->liDueTimeAlarmSound.QuadPart = -(((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_sec * 30000000);
			SetWaitableTimer(pThis->mAlarmSoundTimer, &pThis->liDueTimeAlarmSound, 0, NULL, NULL, FALSE);
			ResetEvent(pThis->mAlarmEvent);
			break;
		case TURNOFFLED_EVENT_NUM:
			ResetEvent(pThis->mReplyTurnOffLEDEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x06, 0x02, 0x00, NULL);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyTurnOffLEDEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyTurnOffLED();
				ResetEvent(pThis->mReplyTurnOffLEDEvent);
			}
			else
			{
				pThis->RecoveState();
			}
			ResetEvent(pThis->mTurnOffLEDEvent);
			break;
		case TURNONLED_EVENT_NUM:
			ResetEvent(pThis->mReplyTurnOnLEDEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x06, 0x01, 0x00, NULL);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyTurnOnLEDEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyTurnOnLED();
				ResetEvent(pThis->mReplyTurnOnLEDEvent);
			}
			else
			{
				pThis->RecoveState();
			}
			CancelWaitableTimer(pThis->mLEDTimer);
			pThis->liDueTimeLED.HighPart = 0;
			pThis->liDueTimeLED.LowPart = 0;
			pThis->liDueTimeLED.QuadPart = -600000000;
			SetWaitableTimer(pThis->mLEDTimer, &pThis->liDueTimeLED, 0, NULL, NULL, FALSE);
			ResetEvent(pThis->mTurnOnLEDEvent);
			break;
		case SETVOLUME_EVENT_NUM:
			ResetEvent(pThis->mReplySetVolumeEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x01, pThis->mCurrentInt[SETVOLUME_EVENT_NUM], 0x00, pThis->mCurrentpDev[SETVOLUME_EVENT_NUM]);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplySetVolumeEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplySetVolume(pThis->mCurrentpDev[SETVOLUME_EVENT_NUM], pThis->mCurrentInt[SETVOLUME_EVENT_NUM]);
				ResetEvent(pThis->mReplySetVolumeEvent);
			}
			else
			{
				Print("SetVolume超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mSetVolumeEvent);
			break;
		case SETLED_EVENT_NUM:
			ResetEvent(pThis->mReplySetLEDEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x07, 0x01, pThis->mCurrentInt[SETLED_EVENT_NUM], NULL);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplySetLEDEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplySetLED(pThis->mCurrentBool[SETLED_EVENT_NUM]);
				ResetEvent(pThis->mReplySetLEDEvent);
			}
			else
			{
				Print("SetLED超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mSetLEDEvent);
			break;
		case HANDLE_EVENT_NUM:
			ResetEvent(pThis->mReplyHandleEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x05, 0x01, 0x00, NULL);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyHandleEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyHandle(pThis->mCurrentBool);
				ResetEvent(pThis->mReplyHandleEvent);
			}
			else
			{
				Print("HANDLE超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mHandleEvent);
			break;
		case GETPORTMAC_EVENT_NUM:
			ResetEvent(pThis->mReplyGetPortMacEvent);
			CUtil::LoadOrder(pThis->mOrder, 0x24, 0x01, 0x02, 0x05, 0x02, pThis->mCurrentInt[GETPORTMAC_EVENT_NUM], NULL);
			CSerialPort::GetInstance(COM_CAM)->WriteToPort(pThis->mOrder, 17);
			pThis->SetWaitReplyState();
			Event = WaitForSingleObject(pThis->mReplyGetPortMacEvent, TIMEOUT_TIME);
			if (Event == WAIT_OBJECT_0)
			{
				pThis->mState->ReplyGetPortMac(pThis->mCurrentInt[GETPORTMAC_EVENT_NUM]);
				ResetEvent(pThis->mReplyGetPortMacEvent);
			}
			else
			{
				Print("GETPORTMAC超时了");
				pThis->RecoveState();
			}
			ResetEvent(pThis->mGetPortMacEvent);
			break;
		case TIMER_ALARM_STOP:
			pThis->StopAlarm(CRecordAlarmSound::GetInstance()->m_pPlayCamera);
			CancelWaitableTimer(pThis->mAlarmSoundTimer);
			pThis->liDueTimeAlarmSound.QuadPart = 0;
			pThis->liDueTimeAlarmSound.LowPart = ftUTC.dwLowDateTime;
			pThis->liDueTimeAlarmSound.HighPart = ftUTC.dwHighDateTime;
			SetWaitableTimer(pThis->mAlarmSoundTimer, &pThis->liDueTimeAlarmSound, 99999999999, NULL, NULL, FALSE);
			break;
		case TIMER_LED_STOP:
			pThis->TurnOffLED();
			CancelWaitableTimer(pThis->mLEDTimer);
			pThis->liDueTimeLED.QuadPart = 0;
			pThis->liDueTimeLED.LowPart = ftUTC.dwLowDateTime;
			pThis->liDueTimeLED.HighPart = ftUTC.dwHighDateTime;
			SetWaitableTimer(pThis->mLEDTimer, &pThis->liDueTimeLED, 99999999999, NULL, NULL, FALSE);
			break;

		}
	}
	return 0;
}

BOOL CCommunication::Init(CWnd * pOwner)
{
	ASSERT(pOwner != NULL);

	st.wYear = 4000; // 年
	st.wMonth = 8;    // 月
	st.wDayOfWeek = 0;    // 一周中的某个星期
	st.wDay = 8;    // 日
	st.wHour = 20;   // 小时（下午8点）
	st.wMinute = 8;    // 分
	st.wSecond = 0;    // 秒
	st.wMilliseconds = 0;    // 毫秒

	SystemTimeToFileTime(&st, &ftLocal);
	LocalFileTimeToFileTime(&ftLocal, &ftUTC);

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

	if (mTurnOnLEDEvent != NULL)
		ResetEvent(mTurnOnLEDEvent);
	else
		mTurnOnLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mTurnOffLEDEvent != NULL)
		ResetEvent(mTurnOffLEDEvent);
	else
		mTurnOffLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

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

	mEventArray[END_EVENT_NUM] = mEndEvent;
	mEventArray[CAMERACANTALK_EVENT_NUM] = mReplyCameraAskTalkEvent;
	mEventArray[STOPTALK_EVENT_NUM] = mStopTalkEvent;
	mEventArray[STOPALARM_EVENT_NUM] = mStopAlarmEvent;
	mEventArray[HOSTTALK_EVENT_NUM] = mHostTalkEvent;
	mEventArray[CAMERATALK_EVENT_NUM] = mCameraTalkEvent;
	mEventArray[ALARM_EVENT_NUM] = mAlarmEvent;
	mEventArray[TURNOFFLED_EVENT_NUM] = mTurnOffLEDEvent;
	mEventArray[TURNONLED_EVENT_NUM] = mTurnOnLEDEvent;
	mEventArray[SETVOLUME_EVENT_NUM] = mSetVolumeEvent;
	mEventArray[SETLED_EVENT_NUM] = mSetLEDEvent;
	mEventArray[HANDLE_EVENT_NUM] = mHandleEvent;
	mEventArray[GETPORTMAC_EVENT_NUM] = mGetPortMacEvent;

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

	if (mReplyTurnOffLEDEvent != NULL)
		ResetEvent(mReplyTurnOffLEDEvent);
	else
		mReplyTurnOffLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mReplyTurnOnLEDEvent != NULL)
		ResetEvent(mReplyTurnOnLEDEvent);
	else
		mReplyTurnOnLEDEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

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

	if (mAlarmSoundTimer != NULL)
		ResetEvent(mAlarmSoundTimer);
	else
		mAlarmSoundTimer = CreateWaitableTimer(NULL, TRUE, NULL);

	if (mLEDTimer != NULL)
		ResetEvent(mLEDTimer);
	else
		mLEDTimer = CreateWaitableTimer(NULL, TRUE, NULL);

	mEventArray[TIMER_ALARM_STOP] = mAlarmSoundTimer;
	mEventArray[TIMER_LED_STOP] = mLEDTimer;
	return 0;
}

BOOL CCommunication::StartThread()
{
	if (!(mThread = AfxBeginThread(CommunicationThread, this)))
		return FALSE;
	return TRUE;
}

CCommunication::~CCommunication()
{
	SetEvent(mEndEvent); 
	CloseHandle(mAlarmSoundTimer);  
	CloseHandle(mLEDTimer);
}

void CCommunication::HostTalk(CCamera *pDev)
{
	this->mCurrentpDev[HOSTTALK_EVENT_NUM] = pDev;
	this->mState->HostTalk(this->mCurrentpDev[HOSTTALK_EVENT_NUM]);
}

void CCommunication::CameraTalk()
{
	mCurrentpDev[CAMERATALK_EVENT_NUM] = mPdev;
	this->mState->CameraTalk(mPdev);
}

void CCommunication::StopTalk()
{
	this->mState->StopTalk();
}

void CCommunication::Alarm(CCamera *pDev)
{
	if (((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_onoff)
	{
		this->mCurrentpDev[ALARM_EVENT_NUM] = pDev;
		this->mState->Alarm(pDev);
	}
}

void CCommunication::StopAlarm(CCamera *pDev)
{
	this->mCurrentpDev[STOPALARM_EVENT_NUM] = pDev;
	this->mState->StopAlarm(pDev);
}

void CCommunication::SetVolume(CCamera *pDev, int Volume)
{
	this->mCurrentpDev[SETVOLUME_EVENT_NUM] = pDev;
	this->mCurrentInt[SETVOLUME_EVENT_NUM] = Volume;
	this->mState->SetVolume(pDev, Volume);
}

void CCommunication::TurnOnLED()
{
	if (((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_light_onoff)
	{
		this->mState->TurnOnLED();
	}
}

void CCommunication::TurnOffLED()
{
	this->mState->TurnOffLED();
}

void CCommunication::SetLED(int isON)
{
	this->mCurrentInt[SETLED_EVENT_NUM] = isON;
	this->mState->SetLED(isON);
}

void CCommunication::Handle()
{
	Print("HANDLE");
	this->mState->Handle();
}

void CCommunication::GetPortMac(int port)
{
	this->mCurrentInt[GETPORTMAC_EVENT_NUM] = port;
	this->mState->GetPortMac(port);
}
//
//void CCommunication::CameraCanTalk(CCamera * pDev)
//{
//	mState->CameraCanTalk(pDev);
//}

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

void CCommunication::ReplyTurnOnLED()
{
	SetEvent(mReplyTurnOnLEDEvent);
}

void CCommunication::ReplyTurnOffLED()
{
	SetEvent(mReplyTurnOffLEDEvent);
}

void CCommunication::ReplySetLED(bool isSucceed)
{
	this->mCurrentBool[SETLED_EVENT_NUM] = isSucceed;
	SetEvent(mReplySetLEDEvent);
}

void CCommunication::ReplyHandle(bool isSucceed)
{
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
	Print("free state");
	mOldState = mState;
	mState = FreeState;
}

void CCommunication::SetAlarmState()
{
	Print("alarm state");
	mOldState = mState;
	mState = AlarmState;
}

void CCommunication::SetCameraTalkState()
{
	Print("camera talk state");
	mOldState = mState;
	mState = CameraTalkState;
}

void CCommunication::SetHostTalkState()
{
	Print("host talk state");
	mOldState = mState;
	mState = HostTalkState;
}

void CCommunication::SetWaitReplyState()
{
	Print("wait reply state");
	mOldState = mState;
	mState = WaitReplyState;
}

void CCommunication::RecoveState()
{
	Print("recove state");
	if (mOldState == WaitReplyState)
		return;
	IState* tmp;
	tmp = mState;
	mState = mOldState;
	mOldState = tmp;
}
