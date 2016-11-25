#include "stdafx.h"
#include "RecordFileMetabolism.h"
#include "Database\DBShadow.h"
#include "Pattern\Observer.h"
#include <iostream> 
#include <list>

CRecordFileMetabolism::~CRecordFileMetabolism()
{
}

bool CRecordFileMetabolism::SetATrigger()
{
	if (!AfxBeginThread(FileMetabolismThread, this))
	{
		return true;
	}
	return false;
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
	for (; iter != CDBShadow::GetInstance()->mAlarmFileInfoList.end(); ++iter)
	{
		if (!((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
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
		}
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::KillNormalFile()
{
	list<CRecordFileInfo*>::iterator iter = CDBShadow::GetInstance()->mReocrdFileInfoList.begin();
	for (; iter != CDBShadow::GetInstance()->mReocrdFileInfoList.end(); ++iter)
	{
		if (!((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
		{
			CTime time = (*iter)->tBegin;
			CString FileName;
			FileName.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), (*iter)->nOwner, time.GetYear(), time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			if (DelFile(_T(NORMAL_RECORD_PATH) + FileName))
			{
				ULONGLONG Size = (*iter)->dlSize;
				Notify(FILE_OPT_DEL, RECORD_NORMAl, (LPARAM)(*iter));
				return Size;
			}
		}
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::GetDiskFreeSpaceAsB(CString DiskName)
{
	ULARGE_INTEGER space;
	if (GetDiskFreeSpaceEx(DiskName, 0, &space, 0))
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
		if (!((*iter)->status == RECORD_LOCKED || (*iter)->bIsOccupied))
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

UINT CRecordFileMetabolism::FileMetabolismThread(LPVOID pParam)
{
	printf("FileMetabolismThread\n");
	CRecordFileMetabolism *pRecordFileMetabolism = (CRecordFileMetabolism*)pParam;
	pRecordFileMetabolism->mSurplusSpaceNormal = pRecordFileMetabolism->GetDiskFreeSpaceAsB(NORMALDISK);
	pRecordFileMetabolism->mSurplusSpaceAlarm = pRecordFileMetabolism->GetDiskFreeSpaceAsB(ALARMDISK);
	while( pRecordFileMetabolism->mSurplusSpaceNormal < SURPLUSSPACENORMAL && pRecordFileMetabolism->mSurplusSpaceNormal)
	{
		uint64_t tmp = pRecordFileMetabolism->KillNormalFile();
		if (tmp == 0)
		{
			break;
		}
		pRecordFileMetabolism->mSurplusSpaceNormal -= tmp;
	}
	while (pRecordFileMetabolism->mSurplusSpaceAlarm < SURPLUSSPACEALARM && pRecordFileMetabolism->mSurplusSpaceAlarm)
	{
		uint64_t tmp = pRecordFileMetabolism->KillAlarmFile();
		if (tmp == 0)
		{
			break;
		}
		pRecordFileMetabolism->mSurplusSpaceAlarm -= tmp;
	}
	//while (pRecordFileMetabolism->IsTimeOutNormalFile())
	//{
	//	pRecordFileMetabolism->KillNormalFile();
	//}
	AfxEndThread(100);
	return 0;
}
