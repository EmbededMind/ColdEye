#include "stdafx.h"
#include "RecordFileMetabolism.h"
#include "Database\DBShadow.h"
#include "Pattern\Observer.h"
#include <iostream> 
#include <list>

CRecordFileMetabolism::~CRecordFileMetabolism()
{
	DeleteCriticalSection(&g_cs);
}


void CRecordFileMetabolism::Notify(UINT opt, WPARAM wParam, LPARAM lParam)
{
	list<Observer*>::iterator iter = this->mObserveres.begin();

	for (; iter != mObserveres.end(); iter++) {
		(*iter)->Update(opt, wParam, lParam);
	}
}

bool CRecordFileMetabolism::DelFile(CString DelPath)
{
	WIN32_FIND_DATA findFileData;
	HANDLE hfind = ::FindFirstFile(DelPath, &findFileData);
	if (hfind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DeleteFile(DelPath);
	hfind = ::FindFirstFile(DelPath, &findFileData);
	if (hfind == INVALID_HANDLE_VALUE)
	{
		printf("delete succeed!\n");
		return true;
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::KillAlarmFile()
{
	list<CRecordFileInfo*>::iterator iter = CDBShadow::GetInstance()->mAlarmFileInfoList.begin();
	for (; iter != CDBShadow::GetInstance()->mAlarmFileInfoList.end(); iter++)
	{
		if ((*iter) && !((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
		{
			CTime time = (*iter)->tBegin;
			CString FileName;
			FileName.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), (*iter)->nOwner, time.GetYear(), time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			if (DelFile(_T(ALARM_RECORD_PATH) + FileName))
			{
				ULONGLONG Size =  (*iter)->dlSize;
				Notify(FILE_OPT_DEL, RECORD_ALARM, (LPARAM)(*iter));
				return Size;
			}
			else
			{
				printf("the file is no exist, delete the info\n");
				CRecordFileInfo* delFileInfo = (*iter);
				iter++;
				Notify(FILE_OPT_DEL, RECORD_ALARM, (LPARAM)(delFileInfo));
			}
		}
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::KillNormalFile()
{
	list<CRecordFileInfo*>::iterator iter = CDBShadow::GetInstance()->mReocrdFileInfoList.begin();
	for (; iter != CDBShadow::GetInstance()->mReocrdFileInfoList.end(); iter++)
	{
		if ((*iter) && !((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
		{
			CTime time = (*iter)->tBegin;
			CString FileName;
			FileName.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), (*iter)->nOwner, time.GetYear(), time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			if (DelFile(_T(NORMAL_RECORD_PATH) + FileName))
			{
				ULONGLONG Size = (*iter)->dlSize;
				CRecordFileInfo* delFileInfo = (*iter);
				Notify(FILE_OPT_DEL, RECORD_NORMAl, (LPARAM)(delFileInfo));
				return Size;
			}
			else
			{
				printf("the file is no exist, delete the info\n");
				CRecordFileInfo* delFileInfo = (*iter);
				iter++;
				Notify(FILE_OPT_DEL, RECORD_NORMAl, (LPARAM)(delFileInfo));
			}
		}
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::GetDiskFreeSpaceAsB(CString DiskName)
{
	ULARGE_INTEGER space;
	if (GetDiskFreeSpaceEx(DiskName, 0, 0, &space))
	{
		return space.QuadPart;
	}
	return 0;
}

bool CRecordFileMetabolism::IsTimeOutNormalFile()
{
	list<CRecordFileInfo*>::iterator iter = CDBShadow::GetInstance()->mReocrdFileInfoList.begin();
	for (; iter != CDBShadow::GetInstance()->mReocrdFileInfoList.end(); ++iter)
	{
		if ((*iter) && !((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
		{
			CTime FileTime = (*iter)->tBegin;
			CTimeSpan TimeSpan = CTime::GetCurrentTime() - FileTime;
			if (TimeSpan.GetTotalMinutes() >= NORMAL_TIME)
			{
				return true;
			}
		}
	}
	return false;
}

BOOL CRecordFileMetabolism::FileMetabolism()
{
	//EnterCriticalSection(&g_cs);
	this->mSurplusSpaceNormal = this->GetDiskFreeSpaceAsB(_T(NORMALDISK));
	Print("2G = %llu byte", SURPLUSSPACENORMAL);
	Print("mSurplusSpaceNormal = %llu", this->mSurplusSpaceNormal);
	while(this->mSurplusSpaceNormal < SURPLUSSPACENORMAL && this->mSurplusSpaceNormal)
	{
		uint64_t tmp = this->KillNormalFile();
		if (tmp == 0)
		{
			break;
		}
		this->mSurplusSpaceNormal += tmp;
	}
	this->mSurplusSpaceAlarm = this->GetDiskFreeSpaceAsB(_T(ALARMDISK));
	Print("mSurplusSpaceAlarm = %llu", this->mSurplusSpaceAlarm);
	while (this->mSurplusSpaceAlarm < SURPLUSSPACEALARM && this->mSurplusSpaceAlarm)
	{
		uint64_t tmp = this->KillAlarmFile();
		if (tmp == 0)
		{
			break;
		}
		this->mSurplusSpaceAlarm += tmp;
	}
	//LeaveCriticalSection(&g_cs);
	return 0;
}
