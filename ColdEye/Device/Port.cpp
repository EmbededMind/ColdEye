#include "stdafx.h"

#include "Port.h"
#include "Database\DBUtil.h"
#include "Pattern\MsgSquare.h"


CString PortName[24] = {  //��Ӧ����id
	_T("�����1"),              
	_T("�����2"),              
	_T("�����3"),              
	_T("�����4"),             
	_T("�����5"),              
	_T("�����6"),              
	_T("��ͷ�����"),           
	_T("���װ������"),         
	_T("�����������"),         
	_T("�����������"),         
	_T("����"),              
	_T("������"),              
	_T("����̨"),             
	_T("���̨"),              
	_T("������"),             
	_T("��Ա��"),              
	_T("������"),              
	_T("�������"),            
	_T("�޾��װ�"),            
	_T("һ��װ�"),           
	_T("����װ�"),           
	_T("����װ�"),            
	_T("�Ĳ�װ�"),            
	_T("���װ�")             
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