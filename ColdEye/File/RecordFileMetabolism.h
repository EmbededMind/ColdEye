#pragma once
#include "stdafx.h"
#include "File\RecordFileButler.h"

class CRecordFileMetabolism:
	public Subject
{
	~CRecordFileMetabolism();
private:
	CRecordFileMetabolism(){};
public:
	static CRecordFileMetabolism* GetInstance() {
		static CRecordFileMetabolism instance;
		return &instance;
	};
	bool SetATrigger();
	virtual void Notify(UINT opt, WPARAM wParam, LPARAM lParam);
private:
	float SurplusSpaceNormal;
	float SurplusSpaceAlarm;
private:
	bool DelFile(CString DelPsth);
	ULONGLONG KillAlarmFile();
	ULONGLONG KillNormalFile();
	ULONGLONG GetDiskFreeSpaceAsMB(CString DiskName);
	bool IsTimeOutNormalFile();
	static UINT FileMetabolismThread(LPVOID pParam);
};

