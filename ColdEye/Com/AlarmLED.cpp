#include "stdafx.h"
#include "AlarmLED.h"
#include "Com\Communication.h"

CAlarmLED::CAlarmLED()
{
}

CAlarmLED::~CAlarmLED()
{
}

void CAlarmLED::TurnOnLED()
{
}

void CAlarmLED::SetMyTimer()
{
	mTimeID = SetTimer(NULL, NULL, 60000, TimeProc);
}

void CAlarmLED::TimeProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	Print("TimeProc");
	KillTimer(NULL, CAlarmLED::GetInstance()->mTimeID);
	CCommunication::GetInstance()->ControlLED(0);
}
