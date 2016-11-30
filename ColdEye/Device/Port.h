#pragma once
  
#include "Device\Camera.h"


#define DEV_CONFIG_MASK_CAM_SW            0x00000001
#define DEV_CONFIG_MASK_RD_SW             0x00000002
#define DEV_CONFIG_MASK_AW_SW             0x00000004
#define DEV_CONFIG_MASK_VOL               0x00000008
#define DEV_CONFIG_MASK_NAME              0x00000010        


#define PORT_EVENT_PLUG                   0x01
#define PORT_EVENT_PULL                   0x02
#define PORT_EVENT_CHANGE                 0x03

#define PORT_STATUS_ONLINE                0x01
#define PORT_STATUS_OFFLINE               0x00 


typedef struct {
	DWORD       Begining;
	DWORD       End;
	//bool        IsAlarmSoundOn;
	//bool        IsAlarmLightOn;
	//uint16_t    AlarmSoundId;
}AutoWatchConfig;


typedef struct {
	bool				IsCameraOn;
	bool				IsRecordEnabled;
	bool				IsAutoWatchEnabled;
	bool				IsVideoFlip;
	uint16_t            Volumn;
	uint16_t            NameId;
}DeviceConfig;


typedef enum {
	OFFLINE = 0,
	PENDING_MAC,
	PENDING_CAMERA,
	PENDING_LOGIN,
	ONLINE
}PortState;


typedef enum {
    OPT_NONE  = 0,
	PEND_MAC,
	SEARCH_CAMERA,
	PEND_LOGIN,
	DROWN,
	RESET
}PortOption;


class CPort
{
public:
	CPort();
	~CPort();
	uint16_t               m_Pos;
	uint16_t               m_Id;
	DeviceConfig           m_DevConfig;
	AutoWatchConfig        m_AwConfig;
	CCamera*               m_pCamera;
	int                    m_virginNumber;

	PortState m_State;

	void  SetId(uint16_t id);
	void  SetNameId(uint16_t id);
	void  SetCamera(CCamera* pCamera);
	void  SetMac(char* sMac);
	void  SetMac(UCHAR* pchMac);
	void  SetVisibility(bool isVisible);
	void  SetActivate(bool newState = false);

	bool  SetAwTime(DWORD tBegining, DWORD tEnd);

	void  StoreParam();

	uint16_t   GetId();	
	uint16_t   GetNameId();
	CCamera*   GetCamera();
    CString&   GetName();
    char*      GetMac();
	bool       GetVisibility();

	bool       IsActive();

	void       Config(DeviceConfig& config);

	PortOption  ParseState(bool isOnline, bool isReplaced);
	PortOption  ParseState( UCHAR* pMac);
	PortOption  ParseState(CCamera* pCamera);


private:
	char m_mac[40];             //存储从主机103读取的mac
	bool m_bIsVisible;
	bool m_bIsActive;

};