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