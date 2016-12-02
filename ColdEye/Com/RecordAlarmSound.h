﻿#pragma once
#include "H264Play.h"
#include "Device\Camera.h"
class CRecordAlarmSound
{
public:
	~CRecordAlarmSound();
private:CRecordAlarmSound() {
	H264_PLAY_GetPort(&m_port);
	m_pTalkDecodeBuf = nullptr;
	m_isAlarm = false;
	m_TimeIdStopAlarm = 0;
	m_pThis = this;
};
public:
	static CRecordAlarmSound* GetInstance() {
		static CRecordAlarmSound instance;
		sprintf_s(instance.m_rName,"%s",RECORD_VOICE_NAME);
		sprintf_s(instance.m_rtmpName,"%s",RECORD_VOICE_NAME_TMP);
		sprintf_s(instance.m_Name, "%s", ALARM_VOICE_NAME);
		return &instance;
	}
public:
	LPBYTE *pBuf; //从文件中 读出音频 的缓冲区
	int len;
	int num;
	char m_rtmpName[50];
	char m_rName[50];
	char m_Name[50];
	CCamera *m_pPlayCamera;
	UINT m_TimeIdStopAlarm;
	UINT m_TimeIdStopLED;
	void Record(CCamera *pCamera);
	bool Save();
	bool NotSave();
	bool Play(CCamera *pCamera, uint8_t type);
	BOOL StopTalk();
	BOOL SendTalkData(LPBYTE pDataBuffer, DWORD dwDataLength);
	BOOL InputTalkData(BYTE *pBuf, DWORD nBufLen);
	BOOL ScanVoice();
protected:
	static CRecordAlarmSound *m_pThis;
	CCamera *m_pRecordCamera;
	LONG m_TalkHandle;
	LONG m_port;
	BOOL StopTalkPlay(long nPort);
	BYTE *m_pTalkDecodeBuf;
	bool m_isAlarm;
	void SetMyTimer();
	static void CALLBACK MyTimerProcAlarm(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
};

