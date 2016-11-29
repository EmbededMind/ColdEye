#pragma once


#define LOG_POWER_ONOFF   1       
#define LOG_SYS_TIME      2
#define LOG_AUTO_WATCH    3
#define LOG_CAM_ON        4
#define LOG_CAM_OFF       5
#define LOG_CAM_AWON      6
#define LOG_CAM_AWOFF     7


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