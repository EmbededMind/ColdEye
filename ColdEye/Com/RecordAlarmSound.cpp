#include "stdafx.h"
#include "RecordAlarmSound.h"
#include "Com\Alaw_encoder.h"
#include "Com\Communication.h"
#include "shlwapi.h"

CRecordAlarmSound::~CRecordAlarmSound()
{
	H264_PLAY_FreePort(m_port);
	if (m_pTalkDecodeBuf)
	{
		delete [] m_pTalkDecodeBuf;
	}
}


CRecordAlarmSound* CRecordAlarmSound::m_pThis;


BOOL CRecordAlarmSound::InputTalkData(BYTE * pBuf, DWORD nBufLen)
{
	return H264_PLAY_InputData(m_port, pBuf, nBufLen);
	return 0;
}
BOOL CRecordAlarmSound::ScanVoice()
{
	if (PathFileExists(_T(RECORD_VOICE_NAME)) && PathFileExists(_T(RECORD_VOICE_NAME_WAV)))
	{
		return true;
	}
	return 0;
}
BOOL CRecordAlarmSound::StopTalkPlay(long nPort)
{
	BOOL bPlayOk = FALSE;
	bPlayOk = H264_PLAY_CloseStream(nPort);
	bPlayOk &= H264_PLAY_StopSoundShare(nPort);
	return bPlayOk;
}
void CRecordAlarmSound::SetMyTimer()
{
	if(!m_TimeIdStopAlarm)
		m_TimeIdStopAlarm = SetTimer(NULL ,NULL, 30000, MyTimerProcAlarm);
	//m_TimeIdStopLED = SetTimer(NULL, NULL, 60000, );
}

void CRecordAlarmSound::MyTimerProcAlarm(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	KillTimer(NULL, m_pThis->m_TimeIdStopAlarm);
	m_pThis->m_TimeIdStopAlarm = NULL;
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), USER_MSG_STOP_ALARM, 0, (LPARAM)(m_pThis->m_pPlayCamera));
}

BOOL CRecordAlarmSound::StopTalk()
{
	m_isAlarm = false;
	H264_PLAY_StopAudioCapture();//�ر���Ƶ�ɼ�����

	BOOL bPlayOk = FALSE;

	StopTalkPlay(m_port);	//�ر�������,�ѹ���ķ�ʽ�رղ����������رղ���ͨ��
	H264_DVR_StopVoiceCom(m_TalkHandle);	//ֹͣ�����Խ�
	CCommunication::GetInstance()->CleanChannel();
	m_TalkHandle = 0;
	return true;
}
void __stdcall TalkDataCallBack(LONG lTalkHandle, char *pDataBuf, long dwBufSize, char byAudioFlag, long dwUser)
{
	CRecordAlarmSound* pDevice = (CRecordAlarmSound*)dwUser;
	if (pDevice)
	{
		pDevice->InputTalkData((BYTE *)pDataBuf, dwBufSize);
		/*PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), USER_MSG_STOP_ALARM, 0, (LPARAM)(pDevice->m_pPlayCamera));*/
	}
}
void __stdcall AudioDataCallBack(LPBYTE pDataBuffer, DWORD dwDataLength, long nUser)
{
	CRecordAlarmSound* pDevice = (CRecordAlarmSound*)nUser;
	if (pDevice)
	{
		pDevice->SendTalkData(pDataBuffer, dwDataLength);//������Ƶ���ݵ�����ͷ
		FILE *stream;
		fopen_s(&stream, pDevice->m_rtmpName, "ab+");
		fwrite(pDataBuffer, dwDataLength, 1, stream);
		fclose(stream);
	}
}
void CRecordAlarmSound::Record(CCamera *pCamera)
{
	m_pRecordCamera = pCamera;
	DeleteFile(_T(RECORD_VOICE_NAME_TMP));//ɾ���ļ�
	int nLen = 640;
	DWORD dwSampleRate = 8000;
	DWORD nAudioBit = 16;//�⼸�������ǲ����ʵ���˼
	BOOL bRet = H264_PLAY_StartAudioCapture(AudioDataCallBack, nAudioBit, dwSampleRate, nLen, (long)this);//�ص����ִ������ļ�,��ʼ¼Ӱ
																										  //bRet = StartTalkPlay(499);//����Ǵ�����ͷ�����ģ�¼���ǲ���Ҫ��
	long lHandle = H264_DVR_StartVoiceCom_MR(pCamera->GetLoginId(), TalkDataCallBack, (DWORD)this);//�������������Խ�����������ת�������ܴ�����ͷ������������
	CCommunication::GetInstance()->mTalkHandle = lHandle;
	CCommunication::GetInstance()->mPdev = pCamera;
	if (lHandle <= 0)//������Ƶ
	{
		//stop audio data capture
		H264_PLAY_StopAudioCapture();//ֹͣ¼��
									 //close decode channel
		StopTalkPlay(m_port);//�ر�������,�ѹ���ķ�ʽ�رղ����������رղ���ͨ��
	}
	else
	{
		m_TalkHandle = lHandle;
	}
}
void __stdcall AudioDataCallBack_2(LPBYTE pDataBuffer, DWORD dwDataLength, long nUser)
{
	CRecordAlarmSound* pDevice = (CRecordAlarmSound*)nUser;
	if (pDevice)
	{
		static unsigned int i = 0;
		//static unsigned int j = 0;
		if (i<pDevice->num - 1)//һ����Ƶ640bit��numͳ���ж��ٶ�
		{
			//if (j >= 4)
			//{
			//	j = 0;
			//	i = 0;
			//	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), USER_MSG_STOP_ALARM, 0, (LPARAM)(pDevice->m_pPlayCamera));
			//}
			pDevice->SendTalkData((LPBYTE)(pDevice->pBuf) + i * 640, 640);//����
			i++;
		}
		else
		{
			i = 0;
			//j++;
		}
	}
}
bool CRecordAlarmSound::Play(CCamera *pCamera, uint8_t type)
{
	Print("Play");
	m_pPlayCamera = pCamera;
	if (!m_isAlarm)
		m_isAlarm = true;
	else
	{
		Print("m_isAlarm == true");
		return 0;
	}
	SetMyTimer();
	int nLen = 640;
	DWORD dwSampleRate = 8000;
	DWORD nAudioBit = 16;//�⼸�������ǲ����ʵ���˼

	FILE *pFile;
	if (type == ALARM_VOICE_DEFAULT)
	{
		if (PathFileExists(_T(RECORD_VOICE_NAME)))
		{
			fopen_s(&pFile, m_Name, "rb");
			Print("ALARM_VOICE_DEFAULT");
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (PathFileExists(_T(ALARM_VOICE_NAME)))
		{
			Print("ALARM_VOICE_RECORD");
			fopen_s(&pFile, m_rName, "rb");
		}
		else
		{
			return 0;
		}
	}
	
	fseek(pFile, 0, SEEK_END);//���ȵ��ļ�ĩβ�鿴�ļ�����
	len = ftell(pFile);//��������Ǿ仰
	pBuf = new LPBYTE[len];
	num = len / nLen;//���ٶΰ���
	rewind(pFile);//�ص��ļ�ͷ
	fread(pBuf, len, 1, pFile);//��ȡ�ļ�

	fclose(pFile); //�����˼��ֱ�Ӵ��ļ��ж�����һ����Ƶ���������¼��

	BOOL bRet = H264_PLAY_StartAudioCapture(AudioDataCallBack_2, nAudioBit, dwSampleRate, nLen, (long)this);//����Ƶ�ɼ�����
	long handle;
	handle = H264_DVR_StartVoiceCom_MR(pCamera->GetLoginId(), TalkDataCallBack, (DWORD)this);//�������������Խ�����������ת��
	CCommunication::GetInstance()->mTalkHandle = handle;
	CCommunication::GetInstance()->mPdev = pCamera;
	if (handle <= 0)//������Ƶ
	{
		//stop audio data capture
		H264_PLAY_StopAudioCapture();
		//close decode channel
		StopTalk();
	}
	else
	{
		m_TalkHandle = handle;
	}
	return false;
}

bool CRecordAlarmSound::Save()
{
	if (PathFileExists(_T(RECORD_VOICE_NAME_TMP)))
	{
		DeleteFile(_T(RECORD_VOICE_NAME));
		CFile::Rename(_T(RECORD_VOICE_NAME_TMP), _T(RECORD_VOICE_NAME));
	}
	return false;
}

bool CRecordAlarmSound::NotSave()
{
	DeleteFile(_T(RECORD_VOICE_NAME_TMP));
	return false;
}

BOOL CRecordAlarmSound::SendTalkData(LPBYTE pDataBuffer, DWORD dwDataLength)
{
	if (NULL == m_pTalkDecodeBuf)
	{
		m_pTalkDecodeBuf = new BYTE[4096];
	}
	memset(m_pTalkDecodeBuf, 0, 4096);

	int iCbLen = 0;

	//encode talk data
	if (g711a_Encode((char*)pDataBuffer, (char*)m_pTalkDecodeBuf + 8, dwDataLength, &iCbLen) != 1)
	{
		ASSERT(FALSE);
		return FALSE;
	}


	m_pTalkDecodeBuf[0] = 0x00;
	m_pTalkDecodeBuf[1] = 0x00;
	m_pTalkDecodeBuf[2] = 0x01;
	m_pTalkDecodeBuf[3] = 0xFA;

	m_pTalkDecodeBuf[4] = 0x0E; //G711A
	m_pTalkDecodeBuf[5] = 0x02;
	m_pTalkDecodeBuf[6] = BYTE(iCbLen & 0xff);
	m_pTalkDecodeBuf[7] = BYTE(iCbLen >> 8);

	iCbLen += 8;
	if (m_TalkHandle)
		H264_DVR_VoiceComSendData(m_TalkHandle, (char*)m_pTalkDecodeBuf, iCbLen);
}
