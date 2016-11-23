#include "stdafx.h"

#include "Port.h"
#include "Database\DBUtil.h"
#include "Pattern\MsgSquare.h"


CString PortName[24] = {  //¶ÔÓ¦Ãû×Öid
	_T("ÉãÏñ»ú1"),              
	_T("ÉãÏñ»ú2"),              
	_T("ÉãÏñ»ú3"),              
	_T("ÉãÏñ»ú4"),             
	_T("ÉãÏñ»ú5"),              
	_T("ÉãÏñ»ú6"),              
	_T("´¬Í·ÉãÏñ»ú"),           
	_T("´¬¼×°åÉãÏñ»ú"),         
	_T("´¬×óÏÏÉãÏñ»ú"),         
	_T("´¬ÓÒÏÏÉãÏñ»ú"),         
	_T("´¬ºó·½"),              
	_T("´¬ÀÂÉþ"),              
	_T("¼¯¿ØÌ¨"),             
	_T("¼à¿ØÌ¨"),              
	_T("´¬³¤ÊÒ"),             
	_T("´¬Ô±ÊÒ"),              
	_T("Ö÷»ú²Õ"),              
	_T("·¢µç»ú²Õ"),            
	_T("ÂÞ¾­¼×°å"),            
	_T("Ò»²ã¼×°å"),           
	_T("¶þ²ã¼×°å"),           
	_T("Èý²ã¼×°å"),            
	_T("ËÄ²ã¼×°å"),            
	_T("Îå²ã¼×°å")             
};


CPort::CPort()
{

}



CPort::~CPort()
{

}



void CPort::SetId(uint16_t id)
{
	m_Id  = id;
}


void CPort::SetNameId(uint16_t id)
{
	m_DevConfig.NameId = id;
}


void CPort::SetCamera(CCamera* pCamera)
{
	m_pCamera  = pCamera;
}



uint16_t CPort::GetId()
{
	return m_Id;
}


uint16_t CPort::GetNameId()
{
	return m_DevConfig.NameId;
}


CCamera* CPort::GetCamera()
{
	return m_pCamera;
}



CString& CPort::GetName()
{
	return PortName[m_DevConfig.NameId-1];
}






void CPort::SetMac(char* sMac)
{
	strcpy_s(m_mac, sMac);
}


void CPort::SetMac(UCHAR* pchMac)
{
	sprintf_s(m_mac, "%02X:%02X:%02X:%02X:%02X:%02X", pchMac[0], pchMac[1], pchMac[2],
		                                              pchMac[3], pchMac[4], pchMac[5]);
}


char* CPort::GetMac()
{
	return m_mac;
}



void CPort::SetVisibility(bool isVisible)
{
	if (m_bIsVisible != isVisible) {
		m_bIsVisible = isVisible;
		
		MSG msg;
		msg.wParam = m_DevConfig.NameId;
		msg.lParam = (LPARAM)this;
		msg.message = USER_MSG_PORT_VISIBILITY_CHANGE;
	}
}



bool CPort::GetVisibility()
{
	return m_bIsVisible;
}


void CPort::SetActivate(bool newState)
{
	this->m_bIsActive  = newState;
}



bool  CPort::IsActive()
{
	return m_bIsActive;
}



void CPort::Config(DeviceConfig& config)
{
	
}