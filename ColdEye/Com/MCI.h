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
		instance.m_rFilePathTmp = _T(RECORD_VOICE_NAME_WAV_TMP);
		instance.m_rFilePath = _T(RECORD_VOICE_NAME_WAV);
		instance.m_FilePath = _T(VOICE_NAME_WAV);
		return &instance;
	};
private:
	CMCI();
private:
	DWORD m_RecordDeviceID;
	DWORD m_PlayDeviceID;
	CString m_rFilePathTmp;
	CString m_rFilePath;
	CString m_FilePath;
	DWORD m_RecordTime;
	DWORD m_PlayTime;
	DWORD m_Totaltime;

	bool m_isPlay;
public:
	int StartRecord();
	int StopRecord();
	int Play(int type);
	int StopPlay();
	int GetRecordTime();
	int GetPlayTime();
	int GetTotaltime();
	BOOL Save();
	BOOL NotSave();
};