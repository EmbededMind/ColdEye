#include "stdafx.h"
#include "MCI.h"


CMCI::CMCI()
{

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
		TRACE("mci open error %d\n",dwReturn);
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
		TRACE("mci save error %d\n",dwReturn);
		return dwReturn;
	}
	mciSendCommand(m_RecordDeviceID, MCI_STOP, NULL, NULL);
	mciSendCommand(m_RecordDeviceID, MCI_CLOSE, NULL, NULL);
	return 0;
}

int CMCI::Play()
{
	MCI_OPEN_PARMS mci_open;
	MCI_PLAY_PARMS mci_play;

	mci_open.lpstrElementName = m_rFilePathTmp;
	DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mci_open);
	if (dwReturn)
	{
		TRACE("mci open error %d\n", dwReturn);
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
	mciSendCommand(m_PlayDeviceID, MCI_STOP, NULL, NULL);
	mciSendCommand(m_PlayDeviceID, MCI_CLOSE, NULL, NULL);
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
	DeleteFile(m_rFilePath);
	CFile::Rename(m_rFilePathTmp, m_rFilePath);
	return 0;
}

BOOL CMCI::NotSave()
{
	mciSendCommand(m_PlayDeviceID, MCI_CLOSE, NULL, NULL);
	DeleteFile(m_rFilePathTmp);
	return 0;
}
