#include "stdafx.h"
#include "MCI.h"
#include "ColdEye.h"

CMCI::CMCI()
{
	m_isPlay = false;
}


CMCI::~CMCI()
{
}

int CMCI::StartRecord()
{
	MCI_OPEN_PARMS mci_open;
	MCI_RECORD_PARMS mci_record;

	mci_open.lpstrDeviceType = _T("waveaudio");
	mci_open.lpstrElementName = _T(""); 
	DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT|MCI_OPEN_TYPE, (DWORD)(LPVOID)&mci_open);
	if (dwReturn)
	{
		Print("mci open error %d\n",dwReturn);
		return dwReturn;
	}
	m_RecordDeviceID = mci_open.wDeviceID;
	mciSendCommand(m_RecordDeviceID, MCI_RECORD, NULL, (DWORD)(LPVOID)&mci_record);
	return 0;
}

int CMCI::StopRecord()
{
	DeleteFile(m_rFilePathTmp);
	MCI_SAVE_PARMS mci_save;
	mci_save.lpfilename = m_rFilePathTmp;
	DWORD dwReturn = mciSendCommand(m_RecordDeviceID, MCI_SAVE, MCI_SAVE_FILE, (DWORD)(LPVOID)&mci_save);
	if (dwReturn)
	{
		Print("mci save error %d\n",dwReturn);
		return dwReturn;
	}
	mciSendCommand(m_RecordDeviceID, MCI_STOP, NULL, NULL);
	mciSendCommand(m_RecordDeviceID, MCI_CLOSE, NULL, NULL);
	return 0;
}

int CMCI::Play(int type)
{
	MCI_OPEN_PARMS mci_open;
	MCI_PLAY_PARMS mci_play;
	if (m_isPlay == true)
		this->StopPlay();
	m_isPlay = true;
	if(type == RECORD_VOICE)
		mci_open.lpstrElementName = m_rFilePath;
	else if(type == RECORD_VOICE_TMP)
		mci_open.lpstrElementName = m_rFilePathTmp;
	else
		mci_open.lpstrElementName = m_FilePath;
	DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mci_open);
	if (dwReturn)
	{
		Print("mci open error %d\n", dwReturn);
		return dwReturn;
	}
	m_PlayDeviceID = mci_open.wDeviceID;
	GetTotaltime();
	mci_play.dwFrom = 0;
	mciSendCommand(m_PlayDeviceID, MCI_PLAY, MCI_FROM, (DWORD)(LPVOID)&mci_play);
	return 0;
}

int CMCI::StopPlay()
{
	if (m_isPlay == false)
		return 0;
	m_isPlay = false;
	DWORD dwReturn = mciSendCommand(m_PlayDeviceID, MCI_STOP, NULL, NULL);
	if (dwReturn)
	{
		Print("mci stop error %d\n", dwReturn);
		return dwReturn;
	}
	dwReturn = mciSendCommand(m_PlayDeviceID, MCI_CLOSE, NULL, NULL);
	if (dwReturn)
	{
		Print("mci close error %d\n", dwReturn);
		return dwReturn;
	}
	return 0;
}

int CMCI::GetRecordTime()
{
	MCI_STATUS_PARMS mci_status;
	mci_status.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(m_RecordDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mci_status);
	m_RecordTime = mci_status.dwReturn / 1000;
	return m_RecordTime;
}

int CMCI::GetPlayTime()
{
	MCI_STATUS_PARMS mci_status;
	mci_status.dwItem = MCI_STATUS_POSITION;
	mciSendCommand(m_PlayDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mci_status);
	m_PlayTime = mci_status.dwReturn / 1000;
	return m_PlayTime;
}

int CMCI::GetTotaltime()
{
	MCI_STATUS_PARMS mci_status;
	memset(&mci_status, 0, sizeof(MCI_STATUS_PARMS));
	mci_status.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(m_PlayDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mci_status);
	m_Totaltime = mci_status.dwReturn / 1000;
	return m_Totaltime;
}

BOOL CMCI::Save()
{
	mciSendCommand(m_PlayDeviceID, MCI_CLOSE, NULL, NULL);
	if (PathFileExists(m_rFilePathTmp))
	{
		DeleteFile(m_rFilePath);
		if (!PathFileExists(m_rFilePath))
		{
			CFile::Rename(m_rFilePathTmp, m_rFilePath);
			((CColdEyeApp*)AfxGetApp())->m_SysConfig.alarm_sound_sec = m_Totaltime;
			((CColdEyeApp*)AfxGetApp())->StoreAlarmSoundConfig();
		}
	}
	return 0;
}

BOOL CMCI::NotSave()
{
	mciSendCommand(m_PlayDeviceID, MCI_CLOSE, NULL, NULL);
	DeleteFile(m_rFilePathTmp);
	return 0;
}
