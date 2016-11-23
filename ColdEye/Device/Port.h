#pragma once
  
#include "Device\Camera.h"

typedef struct {
	DWORD       Begining;
	DWORD       End;
	bool        IsAlarmSoundOn;
	bool        IsAlarmLightOn;
	uint16_t    AlarmSoundId;
}AutoWatchConfig;


typedef struct {
	bool				IsCameraOn;
	bool				IsRecordEnabled;
	bool				IsAutoWatchEnabled;
	bool				IsVideoFlip;
	uint16_t            Volumn;
	uint16_t            NameId;
}DeviceConfig;

class CPort
{
public:
	CPort();
	~CPort();

	uint16_t               m_Id;
	DeviceConfig           m_DevConfig;
	AutoWatchConfig        m_AwConfig;
	CCamera*               m_pCamera;

	void  SetId(uint16_t id);
	void  SetNameId(uint16_t id);
	void  SetCamera(CCamera* pCamera);
	void  SetMac(char* sMac);
	void  SetMac(UCHAR* pchMac);
	void  SetVisibility(bool isVisible);
	void  SetActivate(bool newState = false);


	uint16_t   GetId();	
	uint16_t   GetNameId();
	CCamera*   GetCamera();
    CString&   GetName();
    char*      GetMac();
	bool       GetVisibility();

	bool       IsActive();

	void       Config(DeviceConfig& config);

private:
	char m_mac[20];             //存储从主机103读取的mac
	bool m_bIsVisible;
	bool m_bIsActive;
};