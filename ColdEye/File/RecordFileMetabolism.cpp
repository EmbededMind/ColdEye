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
		return false;
	}
	return true;
}

bool CRecordFileMetabolism::DelFile(CString DelPsth)
{
	WIN32_FIND_DATA findFileData;
	HANDLE hfind = ::FindFirstFile(DelPsth, &findFileData);
	if (hfind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DeleteFile(DelPsth);
	hfind = ::FindFirstFile(DelPsth, &findFileData);
	if (hfind == INVALID_HANDLE_VALUE)
	{
		return true;
	}
	return false;
}

ULONGLONG CRecordFileMetabolism::KillAlarmFile()
{
	list<CRecordFileInfo*>::iterator iter = CDBShadow::GetInstance()->mAlarmFileInfoList.begin();
	for (; iter != CDBShadow::GetInstance()->mAlarmFileInfoList.end(); ++iter)
	{
		if (!((*iter)->bIsLocked || (*iter)->bIsOccupied))
		{
			CTime time = (*iter)->tBegin;
			CString FileName;
			FileName.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), (*iter)->nOwner, time.GetYear(), time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			if (DelFile(_T(ALARM_RECORD_PATH) + FileName))
			{
				ULONGLONG Size =  (*iter)->dlSize;
				///////
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

	}
	return INT64();
}

INT64 CRecordFileMetabolism::GetDiskFreeSpaceAsMB(CString DiskName)
{
	return 0;
}

bool CRecordFileMetabolism::IsTimeOutNormalFile()
{
	return false;
}

UINT CRecordFileMetabolism::FileMetabolismThread(LPVOID pParam)
{
	CRecordFileMetabolism *pRecordFileMetabolism = (CRecordFileMetabolism*)pParam;
	pRecordFileMetabolism->SurplusSpaceNormal = pRecordFileMetabolism->GetDiskFreeSpaceAsMB(NORMALDISK);
	pRecordFileMetabolism->SurplusSpaceAlarm = pRecordFileMetabolism->GetDiskFreeSpaceAsMB(ALARMDISK);
	while( pRecordFileMetabolism->SurplusSpaceNormal < SURPLUSSPACENORMAL)
	{
	}
	while (pRecordFileMetabolism->SurplusSpaceAlarm < SURPLUSSPACEALARM)
	{
	}
	while (pRecordFileMetabolism->IsTimeOutNormalFile())
	{
	}
	AfxEndThread(100);
	return 0;
}
