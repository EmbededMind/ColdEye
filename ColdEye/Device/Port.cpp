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
	//this->m_DevConfig.
}


bool  CPort::SetAwTime(DWORD tBegining, DWORD tEnd)
{
	char sqlStmt[128];
	sprintf_s(sqlStmt, "UPDATE port SET aw_begining = %d, aw_end = %d WHERE id = %d;", tBegining, tEnd,this->m_Id);
	bool bRet =sqlite.DirectStatement(sqlStmt);
	if (bRet == false) {
		Print("Sql error:%s", sqlStmt);
	}
	else {
		m_AwConfig.Begining  = tBegining;
		m_AwConfig.End       = tEnd;
	}

	return bRet;
}





PortOption CPort::ParseState(bool isOnline, bool isReplaced)
{
	if (m_State == OFFLINE) {
		if (isOnline) {
			m_State = PENDING_MAC;

			if (isReplaced) {
				return DROWN;
			}				
			else {
				return PEND_MAC;
			}
		}
	}
}



PortOption CPort::ParseState(bool isOnline, UCHAR* pMac)
{
	if (m_State == PENDING_MAC) {
		sprintf_s(this->m_mac, "%02d:%02d:%02d:%02d:%02d:%02d", pMac);
		m_State  = PENDING_CAMERA;
		return SEARCH_CAMERA;
	}
}



PortOption CPort::ParseState(CCamera* pCamera)
{
	if (m_State == PENDING_CAMERA) {
		m_State  = PENDING_LOGIN;
		m_pCamera  = pCamera;

		return  PEND_LOGIN;
	}
}


