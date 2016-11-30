#include "stdafx.h"

#include "Device\PortManager.h"

#include "Database\DBShadow.h"


CPortManager::CPortManager()
{
	
}



CPortManager::~CPortManager()
{

}


CPortManager* CPortManager::GetInstance()
{
	static CPortManager mInstance;
	return &mInstance;
}


void CPortManager::LoadPortsParam()
{
	char sqlStmt[64];

	sprintf_s(sqlStmt, "SELECT * FROM port;");

	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);

	int i = 0;
	while (stmt->NextRow()) {
		mPorts[i].m_Id                            = stmt->ValueInt(1);
		mPorts[i].m_DevConfig.NameId              = stmt->ValueInt(2);
		mPorts[i].m_DevConfig.IsCameraOn          = stmt->ValueInt(3);
		mPorts[i].m_DevConfig.IsVideoFlip         = stmt->ValueInt(4);
		mPorts[i].m_DevConfig.Volumn              = stmt->ValueInt(5);
		mPorts[i].m_DevConfig.IsRecordEnabled     = stmt->ValueInt(6);
	    mPorts[i].m_DevConfig.IsAutoWatchEnabled  = stmt->ValueInt(7);

		mPorts[i].m_AwConfig.Begining             = stmt->ValueInt(8);
		mPorts[i].m_AwConfig.End                  = stmt->ValueInt(9);
		//mPorts[i].m_AwConfig.IsAlarmSoundOn       = stmt->ValueInt(10);
		//mPorts[i].m_AwConfig.AlarmSoundId         = stmt->ValueInt(11);
		//mPorts[i].m_AwConfig.IsAlarmLightOn       = stmt->ValueInt(12);

		i++;
	}

	CDBShadow* pShadow = CDBShadow::GetInstance();
	for (int i = 0; i < 6; i++) {
		mPorts[i].m_virginNumber = pShadow->GetVirginFileCnt(i + 1);
		Print("Port %d has %d virgin files", i + 1, mPorts[i].m_virginNumber);
	}

	for (int i = 0; i < 6; i++) {
		mPorts[i].m_Pos = i + 1;
	}
}





CPort*  CPortManager::GetPortById(int id)
{
	for (int i = 0; i < 6; i++) {
		if (mPorts[i].GetId() == id) {
			return &mPorts[i];
		}
	}

	return NULL;
}


void CPortManager::BindPortId(CPort* pPort, int id)
{
	for (int i = 0; i < 6; i++) {
		if (&mPorts[i] == pPort) {
			pPort->SetId(id);

			char sqlStmt[128];
			sprintf_s(sqlStmt, "UPDATE port SET id = %d, name_id = %d WHERE pos = %d;", id, pPort->GetNameId(),i+1);

			if (!sqlite.DirectStatement(sqlStmt)) {
				Print("Sql error:%s", sqlStmt);
			}
			else {
				Print("Sql done :%s", sqlStmt);
			}
			return;
		}
	}
}



CPort* CPortManager::AllocPort()
{
	for (int i = 0; i < 6; i++) {
		if (! mPorts[i].IsActive()) {
		     mPorts[i].SetActivate(true);
			 return &mPorts[i];
		}
	}

	return NULL;
}