#pragma once

#include "Device\Port.h"

class CDBLogger
{
private:
	CDBLogger();
	~CDBLogger();

public :
	static CDBLogger* GetInstance();

	void LogPowerOn(CTime& time, int on_off);
	void LogSystemTime(CTime& time);
	void LogAutoWatch(CTime& time, bool on_off);
	void LogCameraOnOff(CTime& time, CPort* pPort);
	void LogCameraAWOnOff(CTime& time, CPort* pPort);

	void GenerateLastPowerOffLog();

	void DeleteOverTimeLog(__time64_t limit_time_oldest, __time64_t limit_time_now);
};