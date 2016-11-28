#pragma once
#include "stdafx.h"
#include "Mmsystem.h"
#include "Digitalv.h"

class CMCI
{
public:
	~CMCI();
	CMCI * GetInstance()
	{
		static CMCI instance;
		return &instance;
	};
private:
	CMCI();
private:
	DWORD m_RecordDeviceID;
	DWORD m_PlayDeviceID;
	CString m_Filepath;
	DWORD m_RecordTime;
	DWORD m_PlayTime;
	DWORD m_Totaltime;

public:
	int Record();
	int Save();
	int Play();
	int StopPlay();
	int GetRecordTime();
	int GetPlayTime();
	int GetTotaltime();
	void SetReocrdPath(LPCTSTR name);
};