#pragma once
#include "H264Play.h"
#include "Device\Camera.h"
class CRecordAlarmSound
{
public:
	~CRecordAlarmSound();
private:CRecordAlarmSound() {
	H264_PLAY_GetPort(&m_port);
	m_pTalkDecodeBuf = nullptr;
};
public:
	static CRecordAlarmSound* GetInstance() {
		static CRecordAlarmSound instance;
		return &instance;
	}
public:
	LPBYTE *pBuf; //���ļ��� ������Ƶ �Ļ�����
	int len;
	int num;
	void Record(CCamera *pCamera);
	bool Save();
	bool NotSave();
	bool Play(CCamera *pCamera);
	BOOL StopTalk();
	BOOL SendTalkData(LPBYTE pDataBuffer, DWORD dwDataLength);
	BOOL InputTalkData(BYTE *pBuf, DWORD nBufLen);
protected:
	CCamera *m_pRecordCamera;
	CCamera *m_pPlayCamera;
	LONG m_TalkHandle;
	LONG m_port;
	BOOL StopTalkPlay(long nPort);
	BYTE *m_pTalkDecodeBuf;
};

