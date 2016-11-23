#include "stdafx.h"
#include "RecordAlarmSound.h"
#include "Com\Alaw_encoder.h"
#include "Com\Communication.h"
CRecordAlarmSound::~CRecordAlarmSound()
{
	H264_PLAY_FreePort(m_port);
	if (m_pTalkDecodeBuf)
	{
		delete [] m_pTalkDecodeBuf;
	}
}
BOOL CRecordAlarmSound::InputTalkData(BYTE * pBuf, DWORD nBufLen)
{
	return H264_PLAY_InputData(m_port, pBuf, nBufLen);
	return 0;
}
BOOL CRecordAlarmSound::StopTalkPlay(long nPort)
{
	BOOL bPlayOk = FALSE;
	bPlayOk = H264_PLAY_CloseStream(nPort);
	bPlayOk &= H264_PLAY_StopSoundShare(nPort);
	return bPlayOk;
}
BOOL CRecordAlarmSound::StopTalk()
{
	H264_PLAY_StopAudioCapture();//关闭音频采集功能

	BOOL bPlayOk = FALSE;

	StopTalkPlay(m_port);	//关闭数据流,已共享的方式关闭播放声音，关闭播放通道
	H264_DVR_StopVoiceCom(m_TalkHandle);	//停止语音对讲
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
	}
}
void __stdcall AudioDataCallBack(LPBYTE pDataBuffer, DWORD dwDataLength, long nUser)
{
	CRecordAlarmSound* pDevice = (CRecordAlarmSound*)nUser;
	if (pDevice)
	{
		pDevice->SendTalkData(pDataBuffer, dwDataLength);//发送音频数据到摄像头
		FILE *stream;
		fopen_s(&stream, "voice_g711a", "ab+");
		fwrite(pDataBuffer, dwDataLength, 1, stream);
		fclose(stream);
	}
}
void CRecordAlarmSound::Record(CCamera *pCamera)
{
	m_pRecordCamera = pCamera;
	DeleteFile(_T("voice_g711a"));//删掉文件
	int nLen = 640;
	DWORD dwSampleRate = 8000;
	DWORD nAudioBit = 16;//这几个参数是采样率的意思
	BOOL bRet = H264_PLAY_StartAudioCapture(AudioDataCallBack, nAudioBit, dwSampleRate, nLen, (long)this);//回调中又创建了文件,开始录影
																										  //bRet = StartTalkPlay(499);//这个是从摄像头回来的，录音是不需要的
	long lHandle = H264_DVR_StartVoiceCom_MR(pCamera->GetLoginId(), TalkDataCallBack, (DWORD)this);//二：开启语音对讲，负责数据转发，接受从摄像头发过来的数据
	CCommunication::GetInstance()->mTalkHandle = lHandle;
	CCommunication::GetInstance()->mPdev = pCamera;
	if (lHandle <= 0)//输入音频
	{
		//stop audio data capture
		H264_PLAY_StopAudioCapture();//停止录音
									 //close decode channel
		StopTalkPlay(m_port);//关闭数据流,已共享的方式关闭播放声音，关闭播放通道
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
		if (i<pDevice->num - 1)//一段音频640bit，num统计有多少段
		{
			pDevice->SendTalkData((LPBYTE)(pDevice->pBuf) + i * 640, 640);//发送
			i++;
		}
		else
			i = 0;
	}
}
bool CRecordAlarmSound::Play(CCamera *pCamera)
{
	int nLen = 640;
	DWORD dwSampleRate = 8000;
	DWORD nAudioBit = 16;//这几个参数是采样率的意思

	FILE *pFile;
	fopen_s(&pFile, "voice_g711a_alarm", "rb");

	fseek(pFile, 0, SEEK_END);//首先到文件末尾查看文件长度
	len = ftell(pFile);//接上面的那句话
	pBuf = new LPBYTE[len];
	num = len / nLen;//多少段啊？
	rewind(pFile);//回到文件头
	fread(pBuf, len, 1, pFile);//读取文件

	fclose(pFile); //这个意思是直接从文件中读出来一段音频，首先你得录好

	BOOL bRet = H264_PLAY_StartAudioCapture(AudioDataCallBack_2, nAudioBit, dwSampleRate, nLen, (long)this);//打开音频采集功能
	long handle;
	handle = H264_DVR_StartVoiceCom_MR(pCamera->GetLoginId(), TalkDataCallBack, (DWORD)this);//二：开启语音对讲，负责数据转发
	CCommunication::GetInstance()->mTalkHandle = handle;
	CCommunication::GetInstance()->mPdev = pCamera;
	if (handle <= 0)//输入音频
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
	DeleteFile(_T("voice_g711a_alarm"));
	CFile::Rename(_T("voice_g711a"), _T("voice_g711a_alarm"));
	return false;
}

bool CRecordAlarmSound::NotSave()
{
	DeleteFile(_T("voice_g711a"));
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
