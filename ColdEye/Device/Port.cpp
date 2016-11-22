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



void CPort::SetId(int id)
{
	this->m_Id = id;
}

int CPort::GetId()
{
	return m_Id;
}


void CPort::SetNameId(int id)
{
	this->m_nameId = id;
}



int CPort::GetNameId()
{
	return m_nameId;
}

CString& CPort::GetName()
{
	return PortName[m_nameId-1];
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
		msg.wParam = m_nameId;
		msg.lParam = (LPARAM)this;
		msg.message = USER_MSG_PORT_VISIBILITY_CHANGE;
	}
}



bool CPort::GetVisibility()
{
	return m_bIsVisible;
}