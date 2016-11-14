#pragma once


class CRecordFileInfo
{
public:
	CRecordFileInfo();
	~CRecordFileInfo();

	UINT       nOwner;
	__time64_t tBegin;
	__time64_t tEnd;
	ULONGLONG  dlSize;
	bool       bIsLocked;
	bool       bIsOccupied;
};
