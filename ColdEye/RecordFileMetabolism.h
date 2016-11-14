#pragma once
#include "stdafx.h"
#include "File\RecordFileInfo.h"
class CRecordFileMetabolism
{
	~CRecordFileMetabolism();
private:
	CRecordFileMetabolism(){};
public:
	static CRecordFileMetabolism* getInstance() {
		static CRecordFileMetabolism instance;
		return &instance;
	};
	void SetATrigger();
private:
	bool DelFile(CFile &RecordFile);
};

