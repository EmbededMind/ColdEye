#pragma once

typedef enum {
	RECORD_NORMAl = 1,
	RECORD_ALARM = 2
}RECORD_FILE_TYPE;


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
