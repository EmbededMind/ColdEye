#include "stdafx.h"

#include "Database\DBLogger.h"
#include "Database\sqlitewrapper.h"




CDBLogger::CDBLogger()
{
}


CDBLogger::~CDBLogger()
{}


CDBLogger* CDBLogger::GetInstance()
{
	static CDBLogger mInstance;
	return &mInstance;
}


void CDBLogger::LogPowerOn(CTime& t, int on_off)
{
	char sqlStmt[64];

	/*sprintf_s(sqlStmt, "INSERT INTO log (time, opt) VALUES (%I64d, %d);", t.GetTime(), LOG_POWER_ON);*/

	sprintf_s(sqlStmt, "insert into log (time, opt, tag) values (%I64d, %d, %d);", t.GetTime(), LOG_POWER_ONOFF, on_off);

	
	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}


void CDBLogger::LogSystemTime(CTime& t)
{
	char sqlStmt[128];


	sprintf_s(sqlStmt, "update log set time = %I64d where opt = 2", t.GetTime());

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
		sprintf_s(sqlStmt, "insert into log (time, opt) values (%I64d, 2)", t.GetTime());

		if (!sqlite.DirectStatement(sqlStmt)) {
			Print("Sql error:%s", sqlStmt);
		}
	}
}



void CDBLogger::LogAutoWatch(CTime& t)
{
	char sqlStmt[128];

	sprintf_s(sqlStmt, "INSERT INTO log VALUES(%I64d, 2);", t.GetTime());

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}


void CDBLogger::LogCameraOnOff(CTime& t, CCamera* pCamera)
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "INSERT INTO log VALUES(%I64d, 4, %d);", t.GetTime(), pCamera->m_LocalConfig.IsActivate);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}


void CDBLogger::LogCameraAWOnOff(CTime& t, CCamera* pCamera)
{
	char sqlStmt[64];
	sprintf_s(sqlStmt, "INSERT INTO log VALUES(%I64d, 5, %d);", t.GetTime(), pCamera->m_LocalConfig.IsAutoWatchEnabled);

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}



void CDBLogger::GenerateLastPowerOffLog()
{
	char sqlStmt[64];
	__time64_t  last_shutdown_time = 0;
	__time64_t  last_shutdown_log_time = 0;

	sprintf_s(sqlStmt,"SELECT * FROM log WHERE opt = 2;");

	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);
	if (stmt->NextRow()) {
		last_shutdown_time = stmt->ValueInt(0);

		if (last_shutdown_time > 0) {
			sprintf_s(sqlStmt, "SELECT max(time) FROM log WHERE opt = 1 AND tag = 0;");
			stmt = sqlite.Statement(sqlStmt);
			if (stmt->NextRow()) {
				last_shutdown_log_time = stmt->ValueInt(0);
			}
		}

		if (last_shutdown_time != last_shutdown_log_time) {
			sprintf_s(sqlStmt, "INSERT INTO log VALUES(%I64d, 1, 0);", last_shutdown_time);

			if (!sqlite.DirectStatement(sqlStmt)) {
				Print("Sql error:%s", sqlStmt);
			}
		}
	}
}


void CDBLogger::DeleteOverTimeLog(__time64_t oldest, __time64_t now)
{
	char sqlStmt[128];

	sprintf_s(sqlStmt, "DELETE FROM log WHERE time < oldest OR time > now;");

	if (!sqlite.DirectStatement(sqlStmt)) {
		Print("Sql error:%s", sqlStmt);
	}
}