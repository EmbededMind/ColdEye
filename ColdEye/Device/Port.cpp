#include "stdafx.h"

#include "Port.h"
#include "Database\DBUtil.h"
#include "Pattern\MsgSquare.h"


CString PortName[24] = {  //��Ӧ����id
	_T("�����1"),              // 1
	_T("�����2"),              // 2
	_T("�����3"),              // 3
	_T("�����4"),              // 4
	_T("�����5"),              // -5
	_T("�����6"),              // -6
	_T("��ͷ�����"),           //  1
	_T("���װ������"),         //  2
	_T("�����������"),         //  3
	_T("�����������"),         //  4
	_T("����"),              //  5
	_T("������"),              //  6
	_T("����̨"),              //  7
	_T("���̨"),              //  8
	_T("������"),              //  9
	_T("��Ա��"),              // 10
	_T("������"),              // 11
	_T("�������"),            // 12
	_T("�޾��װ�"),            // 13
	_T("һ��װ�"),            // 14
	_T("����װ�"),            // 15
	_T("����װ�"),            // 16
	_T("�Ĳ�װ�"),            // 17
	_T("���װ�")             // 18
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