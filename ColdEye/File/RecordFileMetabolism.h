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
	virtual void Notify(UINT opt, WPARAM wParam, LPARAM lParam);
	BOOL FileMetabolism();
private:
	ULONGLONG mSurplusSpaceNormal;
	ULONGLONG mSurplusSpaceAlarm;
private:
	bool DelFile(CString DelPsth);
	ULONGLONG KillAlarmFile();
	ULONGLONG KillNormalFile();
	ULONGLONG GetDiskFreeSpaceAsB(CString DiskName);
	bool IsTimeOutNormalFile();
};

