#pragma once
#include "stdafx.h"

class CAlarmLED
{
public:
	static CAlarmLED* GetInstance()
	{
		static CAlarmLED instance;
		return &instance;
	};

	void TurnOnLED();
	void SetMyTimer();
private:
	CAlarmLED();
	~CAlarmLED();
	UINT mTimeID;

	static void CALLBACK TimeProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
};

