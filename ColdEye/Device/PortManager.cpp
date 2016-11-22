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
		mPorts[i].SetId(stmt->ValueInt(1));
		mPorts[i].SetNameId(stmt->ValueInt(2));
		i++;
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


