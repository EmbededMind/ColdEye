#include "stdafx.h"

#include "File\RecordInfoManager.h"

CRecordInfoManager::CRecordInfoManager()
{
}

CRecordInfoManager::~CRecordInfoManager()
{
}



CRecordInfoManager* CRecordInfoManager::GetInstance()
{
	static CRecordInfoManager mInstance;
	return &mInstance;
}

