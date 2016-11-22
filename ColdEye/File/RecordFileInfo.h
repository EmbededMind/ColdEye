#pragma once

typedef enum {
	RECORD_NORMAl = 1,
	RECORD_ALARM = 2
}RECORD_FILE_TYPE;
//#define RECORD_NORMAL   1
//#define RECORD_ALARM     2



typedef enum {
	RECORD_HASSEEN = 0,
	RECORD_NOTSEEN,
	RECORD_LOCKED
}RECORD_FILE_STATUS;


class CRecordFileInfo
{
public:
	CRecordFileInfo();
	~CRecordFileInfo();

	UINT       nOwner;
	__time64_t tBegin;
	__time64_t tEnd;
	ULONGLONG  dlSize;
	RECORD_FILE_STATUS status;
	bool       bIsOccupied;
};
