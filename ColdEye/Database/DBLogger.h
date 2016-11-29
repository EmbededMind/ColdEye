#pragma once

#include "Device\Camera.h"

class CDBLogger
{
private:
	CDBLogger();
	~CDBLogger();

public :
	static CDBLogger* GetInstance();

	void LogPowerOn(CTime& time, int on_off);
	void LogSystemTime(CTime& time);
	void LogAutoWatch(CTime& time);
	void LogCameraOnOff(CTime& time, CCamera* pCamera);
	void LogCameraAWOnOff(CTime& time, CCamera* pCamera);

	void GenerateLastPowerOffLog();

	void DeleteOverTimeLog(__time64_t limit_time_oldest, __time64_t limit_time_now);
};