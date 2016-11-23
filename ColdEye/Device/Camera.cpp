#include "stdafx.h"

#include "Camera.h"
#include "Com\Util.h"


extern int __stdcall cbRealData(long lRealHandle, const PACKET_INFO_EX* pFrame, UINT dwUser);


CCamera::CCamera()
{
	m_nPort = 34567;
	strcpy_s(m_szUserName, "admin");
	strcpy_s(m_szPassword, "");
	m_ClientInfo.nChannel = 0;
	m_ClientInfo.nMode = 0;
	m_ClientInfo.nStream = 1;
	m_DeviceInfo.deviceTye = SDK_DEVICE_TYPE_DVR;
	

	m_Param.whiteBalance = 0;         //白平衡
	m_Param.dayNightColor = 0;        //日夜模式，取值有彩色、自动切换和黑白
	m_Param.elecLevel = 20;            //参考电平值
	m_Param.apertureMode = 1;         //自动光圈模式
	m_Param.BLCMode = 1;              //背光补偿模式

	//曝光设置
	m_Param.exposureConfig.level  = 20;  //曝光等级
	m_Param.exposureConfig.leastTime = 0; //自动曝光时间下限或手动曝光时间，单位微秒
	m_Param.exposureConfig.mostTime = 80; //自动曝光时间上限，单位微秒

	//增益配置
	m_Param.gainConfig.autoGain = 1; //自动增益是否启用，0:不开启  1:开启
	m_Param.gainConfig.gain = 4;     //自动增益上限(自动增益启用)或固定增益值

	m_Param.PictureFlip = 0;  //上下翻转
	m_Param.PictureMirror = 0; //镜像
	m_Param.RejectFlicker = 0; //日光灯防闪功能
	m_Param.EsShutter = 0; //电子慢快门功能
	m_Param.ircut_mode = 1;//IR-CUT切换 0 = 红外灯同步切换 1 = 自动切换
	m_Param.dnc_thr = 50;//日夜转换阈值
	m_Param.ae_sensitivity = 4;//ae灵敏度配置
	m_Param.Day_nfLevel = 0;//noise filter 等级，0-5,0不滤波，1-5 值越大滤波效果越明显
	m_Param.Night_nfLevel = 0;
	m_Param.Ircut_swap = 0;//ircut 正常序= 0        反序= 1
}

CCamera::~CCamera()
{
}



#ifdef _DEBUG
void   CCamera::DumpParam()
{
	Print("白平衡：%d", m_Param.whiteBalance);
	Print("色彩模式：%d", m_Param.dayNightColor);
	Print("参考电平值：%d", m_Param.elecLevel);
	Print("自动光圈模式：%d", m_Param.apertureMode);
	Print("背光补偿模式：%d", m_Param.BLCMode);
	Print("上下翻转：%d", m_Param.PictureFlip);
	Print("镜像：%d", m_Param.PictureMirror);
	Print("日光灯防闪：%d", m_Param.RejectFlicker);
	Print("电子慢快门：%d", m_Param.EsShutter);
	Print("IR-CUT切换：%d", m_Param.ircut_mode);
	Print("日夜转换阈值：%d", m_Param.dnc_thr);
	Print("ae灵敏度：%d", m_Param.ae_sensitivity);
	Print("白天噪音过滤等级：%d", m_Param.Day_nfLevel);
	Print("夜晚噪音过滤等级：%d", m_Param.Night_nfLevel);
	Print("IR-CUT顺序：%d", m_Param.Ircut_swap);
}


void CCamera::DumpAbility()
{
	Print("支持曝光速度数量：%d", m_Ability.count);
	Print("工作状态：%d", m_Ability.status);
	Print("参考点评：%d", m_Ability.elecLevel);
}
#endif


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
		printf("Login\n");
		CUtil::LoadMap(this);
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
			CUtil::RemoveDev(this);
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
//BOOL CCamera::StartRealPlay()
//{
//	if(m_LoginId  > 0){
//		m_hRealPlay = H264_DVR_RealPlay(m_LoginId, &this->m_ClientInfo);
//		if (m_hRealPlay > 0) {
//			return TRUE;
//		}
//		else {
//			TRACE("Real play failed.Error code:%d\n", H264_DVR_GetLastError());
//			return FALSE;
//		}
//	}
//	else {
//		return FALSE;
//	}
//}


/**@brief 将摄像机实时视频显示在指定窗口
 *
 * @param [in] hWnd,指定用于显示视频的窗口
 * @note  ColdEye 不应调用该函数，只能打开实时视频传输，不要指定实时播放。
 */
//BOOL CCamera::StartRealPlay(HWND hWnd)
//{
//	this->m_ClientInfo.hWnd = hWnd;
//	StartRealPlay();
//	return 0;
//}


//void CCamera::StopRealPlay()
//{
//	if (m_hRealPlay > 0) {
//		if (!H264_DVR_StopRealPlay(m_hRealPlay, &this->m_ClientInfo.hWnd)) {
//			Print("%s stop real play failed:%d", this->m_szIp, H264_DVR_GetLastError());
//			ASSERT(false);
//		}
//		m_hRealPlay = 0;
//		Print("Stop real play");
//	}
//	else {
//		Print("Invalid hRealPlay");
//	}
//
//}



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



void CCamera::SetSDKCameraParam()
{
	ASSERT(m_LoginId > 0);

	long lRet = H264_DVR_SetDevConfig(m_LoginId, E_SDK_CONFIG_CAMERA, 0, 
		(char*)&m_Param, sizeof(SDK_CameraParam), 2000);
	if (lRet < 0) {
		Print("Set camear param failed:%d", H264_DVR_GetLastError());
	}

}


void CCamera::GetSDKCameraParam()
{
	ASSERT(m_LoginId > 0);

	DWORD dwResBytes = 0;

	long lRet = H264_DVR_GetDevConfig(m_LoginId, E_SDK_CONFIG_CAMERA, 0, 
		(char*)&m_Param, sizeof(SDK_CameraParam), &dwResBytes, 2000);

	if (lRet < 0) {
		Print("Get camera param failed:%d", H264_DVR_GetLastError());
	}
#ifdef _DEBUG
	else if (lRet > 0) {
		DumpParam();
	}
#endif
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
	Print("Camera on disconnect");
	Logout();
	//StopRealPlay();
}


//void CCamera::AttachPort(CPort* pPort)
//{
//	this->m_pAttachedPort = pPort;
//}
//
//
//CPort*  CCamera::GetAttachedPort()
//{
//	return this->m_pAttachedPort;
//}