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
	

	m_Param.whiteBalance = 0;         //��ƽ��
	m_Param.dayNightColor = 0;        //��ҹģʽ��ȡֵ�в�ɫ���Զ��л��ͺڰ�
	m_Param.elecLevel = 20;            //�ο���ƽֵ
	m_Param.apertureMode = 1;         //�Զ���Ȧģʽ
	m_Param.BLCMode = 1;              //���ⲹ��ģʽ

	//�ع�����
	m_Param.exposureConfig.level  = 20;  //�ع�ȼ�
	m_Param.exposureConfig.leastTime = 0; //�Զ��ع�ʱ�����޻��ֶ��ع�ʱ�䣬��λ΢��
	m_Param.exposureConfig.mostTime = 80; //�Զ��ع�ʱ�����ޣ���λ΢��

	//��������
	m_Param.gainConfig.autoGain = 1; //�Զ������Ƿ����ã�0:������  1:����
	m_Param.gainConfig.gain = 4;     //�Զ���������(�Զ���������)��̶�����ֵ

	m_Param.PictureFlip = 0;  //���·�ת
	m_Param.PictureMirror = 0; //����
	m_Param.RejectFlicker = 0; //�չ�Ʒ�������
	m_Param.EsShutter = 0; //���������Ź���
	m_Param.ircut_mode = 1;//IR-CUT�л� 0 = �����ͬ���л� 1 = �Զ��л�
	m_Param.dnc_thr = 50;//��ҹת����ֵ
	m_Param.ae_sensitivity = 4;//ae����������
	m_Param.Day_nfLevel = 0;//noise filter �ȼ���0-5,0���˲���1-5 ֵԽ���˲�Ч��Խ����
	m_Param.Night_nfLevel = 0;
	m_Param.Ircut_swap = 0;//ircut ������= 0        ����= 1
}

CCamera::~CCamera()
{
}



#ifdef _DEBUG
void   CCamera::DumpParam()
{
	Print("��ƽ�⣺%d", m_Param.whiteBalance);
	Print("ɫ��ģʽ��%d", m_Param.dayNightColor);
	Print("�ο���ƽֵ��%d", m_Param.elecLevel);
	Print("�Զ���Ȧģʽ��%d", m_Param.apertureMode);
	Print("���ⲹ��ģʽ��%d", m_Param.BLCMode);
	Print("���·�ת��%d", m_Param.PictureFlip);
	Print("����%d", m_Param.PictureMirror);
	Print("�չ�Ʒ�����%d", m_Param.RejectFlicker);
	Print("���������ţ�%d", m_Param.EsShutter);
	Print("IR-CUT�л���%d", m_Param.ircut_mode);
	Print("��ҹת����ֵ��%d", m_Param.dnc_thr);
	Print("ae�����ȣ�%d", m_Param.ae_sensitivity);
	Print("�����������˵ȼ���%d", m_Param.Day_nfLevel);
	Print("ҹ���������˵ȼ���%d", m_Param.Night_nfLevel);
	Print("IR-CUT˳��%d", m_Param.Ircut_swap);
}


void CCamera::DumpAbility()
{
	Print("֧���ع��ٶ�������%d", m_Ability.count);
	Print("����״̬��%d", m_Ability.status);
	Print("�ο�������%d", m_Ability.elecLevel);
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
		CUtil::LoadMap(this);
		return TRUE;
	}
	else
	{
		TRACE("%s login failed. error code:%d\n", m_szIp, iErrCode);
		return FALSE;
	}
}



BOOL CCamera::SetCameraTime(CTime& time)
{
	SDK_SYSTEM_TIME sdkTime;
	sdkTime.year      = time.GetYear();
	sdkTime.month     = time.GetMonth();
	sdkTime.day       = time.GetDay();
	sdkTime.wday      = time.GetDayOfWeek();
	sdkTime.hour      = time.GetHour();
	sdkTime.minute    = time.GetMinute();
	sdkTime.second    = time.GetSecond();
	sdkTime.isdst     = 0;
	
	if (m_LoginId > 0) {
		if(H264_DVR_SetSystemDateTime(m_LoginId, &sdkTime)){
			return TRUE;
		}
		else{
			Print("����ʱ��ʧ��:%d", H264_DVR_GetLastError());
			return FALSE;
		}
	}
	else {
		Print("��Ч��¼���");
	}

	return FALSE;
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
	else {
		Print("Get camear param ok");
#ifdef _DEBUG
		DumpParam();
#endif
	}
}


/**@brief ����ͷ���ı�����Ϣ(�������ܿ�����
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


/**@brief ����ͷȡ�����ı�����Ϣ(�������ܹرգ�
*
* @note:
*/
BOOL CCamera::UnsubscribeAlarmMessage()
{
	if (this->m_LoginId > 0) {
		if (H264_DVR_CloseAlarmChan(this->m_LoginId)) {
			return TRUE;
		}
		else {
			Print("CloseAlarmChann failed:%d", H264_DVR_GetLastError());
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