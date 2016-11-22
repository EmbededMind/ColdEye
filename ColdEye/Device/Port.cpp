#include "stdafx.h"

#include "Port.h"
#include "Database\DBUtil.h"
#include "Pattern\MsgSquare.h"


CString PortName[24] = {  //¶ÔÓ¦Ãû×Öid
	_T("ÉãÏñ»ú1"),              // 1
	_T("ÉãÏñ»ú2"),              // 2
	_T("ÉãÏñ»ú3"),              // 3
	_T("ÉãÏñ»ú4"),              // 4
	_T("ÉãÏñ»ú5"),              // -5
	_T("ÉãÏñ»ú6"),              // -6
	_T("´¬Í·ÉãÏñ»ú"),           //  1
	_T("´¬¼×°åÉãÏñ»ú"),         //  2
	_T("´¬×óÏÏÉãÏñ»ú"),         //  3
	_T("´¬ÓÒÏÏÉãÏñ»ú"),         //  4
	_T("´¬ºó·½"),              //  5
	_T("´¬ÀÂÉþ"),              //  6
	_T("¼¯¿ØÌ¨"),              //  7
	_T("¼à¿ØÌ¨"),              //  8
	_T("´¬³¤ÊÒ"),              //  9
	_T("´¬Ô±ÊÒ"),              // 10
	_T("Ö÷»ú²Õ"),              // 11
	_T("·¢µç»ú²Õ"),            // 12
	_T("ÂÞ¾­¼×°å"),            // 13
	_T("Ò»²ã¼×°å"),            // 14
	_T("¶þ²ã¼×°å"),            // 15
	_T("Èý²ã¼×°å"),            // 16
	_T("ËÄ²ã¼×°å"),            // 17
	_T("Îå²ã¼×°å")             // 18
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
	return PortName[m_nameId];
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