#include "stdafx.h"

#include "Camera.h"


CCamera::CCamera()
{
	m_nPort = 34567;
	strcpy_s(m_szUserName, "admin");
	strcpy_s(m_szPassword, "");
	m_ClientInfo.nChannel = 0;
	m_ClientInfo.nMode = 0;
	m_ClientInfo.nStream = 1;
	m_DeviceInfo.deviceTye = SDK_DEVICE_TYPE_DVR;
}

CCamera::~CCamera()
{
}

void CCamera::GetIp(char* pBuf)
{
	ASSERT(pBuf != nullptr);

	strcpy_s(m_szIp, 20,pBuf);
}


void CCamera::GetUserName(char* pBuf)
{
	ASSERT(pBuf != nullptr);

	strcpy_s(m_szUserName,20, pBuf);
}


void CCamera::GetPassword(char* pBuf)
{
	ASSERT(pBuf != nullptr);

	strcpy_s(m_szPassword, 20,pBuf);
}


DWORD CCamera::GetPort()
{
	return m_nPort;
}

long CCamera::GetLoginId()
{
	return m_LoginId;
}


void CCamera::SetIp(char* szIp)
{
	ASSERT(szIp != nullptr);

	strcpy_s(m_szIp, szIp);
}


void CCamera::SetIp(SDK_CONFIG_NET_COMMON_V2* pCommonNetConfig)
{
	ASSERT(pCommonNetConfig != nullptr);

	int tmp = 0;
	TRACE("%d.%d.%d.%d\n", pCommonNetConfig->HostIP.c[0], pCommonNetConfig->HostIP.c[1], pCommonNetConfig->HostIP.c[2], pCommonNetConfig->HostIP.c[3]);
	for (int j = 0; j < 4; j++){			
		tmp += sprintf_s(&m_szIp[tmp], 4, "%d", pCommonNetConfig->HostIP.c[j]);
		if (j < 3) {
			m_szIp[tmp] = '.';
			tmp++;
		}
	}
}


void CCamera::SetUserName(char* szUserName)
{
	ASSERT(szUserName != nullptr);

	strcpy_s(m_szUserName, szUserName);
}




void CCamera::SetPassword(char* szPwd)
{
	ASSERT(szPwd != nullptr);

	strcpy_s(m_szPassword, szPwd);
}





void CCamera::SetPort(WORD nPort)
{
	m_nPort = nPort;
}



void CCamera::SetCommonNetConfig(SDK_CONFIG_NET_COMMON_V2* pConfig)
{
	memcpy_s(&mCommonNetConfig, sizeof(SDK_CONFIG_NET_COMMON_V2), pConfig, sizeof(SDK_CONFIG_NET_COMMON_V2));
	SetIp(pConfig);
}



BOOL CCamera::SetClientWindow(HWND hWnd)
{
	if (IsWindow(hWnd)) {
		m_ClientInfo.hWnd = hWnd;
		return TRUE;
	}
	
	return FALSE;
}


BOOL CCamera::Login()
{
	int iErrCode = 0;

	long lRet = H264_DVR_Login(m_szIp, m_nPort, m_szUserName, m_szPassword, &m_DeviceInfo, &iErrCode);
	if (lRet > 0)
	{
		m_LoginId = lRet;
		return TRUE;
	}
	else
	{
		TRACE("%s login failed. error code:%d\n", m_szIp, iErrCode);
		return FALSE;
	}
}



void CCamera::OnLogin()
{

}


void CCamera::Logout()
{
	if (m_LoginId > 0) {
		if (H264_DVR_Logout(m_LoginId)) {
			m_LoginId = 0;
		}
		else {
			Print("%s logout failed:%d", m_szIp, H264_DVR_GetLastError());
		}
	}
	else {
		Print("Invalid logid:%d when logout", m_LoginId);
	}
}


/**@brief 摄像机开启实时视频数据传输
 *
 */
BOOL CCamera::StartRealPlay()
{
	if(m_LoginId  > 0){
		m_hRealPlay = H264_DVR_RealPlay(m_LoginId, &this->m_ClientInfo);
		if (m_hRealPlay > 0) {
			return TRUE;
		}
		else {
			TRACE("Real play failed.Error code:%d\n", H264_DVR_GetLastError());
			return FALSE;
		}
	}
	else {
		return FALSE;
	}
}


/**@brief 将摄像机实时视频显示在指定窗口
 *
 * @param [in] hWnd,指定用于显示视频的窗口
 * @note  ColdEye 不应调用该函数，只能打开实时视频传输，不要指定实时播放。
 */
BOOL CCamera::StartRealPlay(HWND hWnd)
{
	this->m_ClientInfo.hWnd = hWnd;
	StartRealPlay();
	return 0;
}


void CCamera::StopRealPlay()
{
	if (m_hRealPlay > 0) {
		if (!H264_DVR_StopRealPlay(m_hRealPlay, &this->m_ClientInfo.hWnd)) {
			Print("%s stop real play failed:%d", this->m_szIp, H264_DVR_GetLastError());
			ASSERT(false);
		}
		m_hRealPlay = 0;
	}
	Print("Invalid hRealPlay");
}



BOOL CCamera::LoadLocalConfig()
{
	m_LocalConfig.IsActivate           = true;
	m_LocalConfig.IsAutoWatchEnabled   = true;
	m_LocalConfig.IsVideoRecordEnabled = true;
	m_LocalConfig.NameIndex            = 0 ;
	m_LocalConfig.VideoDir             = VIDEO_DIR_0;
	m_LocalConfig.Volumn               = DEFAULT_VOL;
	m_LocalConfig.AutoWatchTimeStart   = DEFAULT_AWT_START;
	m_LocalConfig.AutoWatchTimeEnd     = DEFAULT_AWT_END;
	return TRUE;
}


/**@brief 摄像头订阅报警信息(报警功能开启）
*
* @note:
*/
BOOL CCamera::SubscribeAlarmMessage()
{
//Print("SubscribeAlarmMessage");
	if (this->m_LoginId > 0) {
		if (H264_DVR_SetupAlarmChan(this->m_LoginId)) {
			return TRUE;
		}
		else {
			TRACE("SetupAlarmChan failed:%d\n", H264_DVR_GetLastError());
		}
	}
    
	return FALSE;
}


/**@brief 摄像头取消订阅报警信息(报警功能关闭）
*
* @note:
*/
BOOL CCamera::UnsubscribeAlarmMessage()
{
	if (this->m_LoginId > 0) {
		if (!H264_DVR_CloseAlarmChan(this->m_LoginId)) {
			return TRUE;
		}
		else {
			TRACE("CloseAlarmChann failed:%d\n", H264_DVR_GetLastError());
		}
	}

	return FALSE;
}



void CCamera::OnDisConnnect()
{
	Logout();
	StopRealPlay();
}