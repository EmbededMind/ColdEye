#pragma once
#include "stdafx.h"
#include "Mmsystem.h"
#include "Digitalv.h"

class CMCI
{
public:
	~CMCI();
	static CMCI * GetInstance()
	{
		static CMCI instance;
		instance.m_FilePathTmp = _T(RECORD_VOICE_NAME_TMP);
		instance.m_FilePath = _T(RECORD_VOICE_NAME);
		return &instance;
	};
private:
	CMCI();
private:
	DWORD m_RecordDeviceID;
	DWORD m_PlayDeviceID;
	CString m_FilePathTmp;
	CString m_FilePath;
	DWORD m_RecordTime;
	DWORD m_PlayTime;
	DWORD m_Totaltime;

public:
	int StartRecord();
	int StopRecord();
	int Play();
	int StopPlay();
	int GetRecordTime();
	int GetPlayTime();
	int GetTotaltime();
	BOOL Save();
	BOOL NotSave();
};