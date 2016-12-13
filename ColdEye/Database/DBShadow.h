#pragma once

#include "Pattern\Observer.h"
#include "File\RecordFileInfo.h"

#include "Database\sqlitewrapper.h"

using namespace std;

class CDBShadow:
	public Observer
{
private:
	CDBShadow();
	CDBShadow(const CDBShadow&);	
	~CDBShadow();
	CRITICAL_SECTION g_cs;
	static CDBShadow* mInstance_1;

	int mRecordFileCnts[6];
	int mAlarmFileCnts[6];

	int mVirginFileCnts[6];


	void  CheckTables();

	void  AddFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);
	void  EndFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);
	void  DelFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);

public:
	list<CRecordFileInfo*> mReocrdFileInfoList;
	list<CRecordFileInfo*> mAlarmFileInfoList;

	int GetVirginFileCnt(int id);
	int GetVirginFileCnt();
	
	static CDBShadow* GetInstance();


	virtual void Update(UINT opt, WPARAM wParam, LPARAM lParam);

	//void Init();
	void SynchronizeWithDB();	

	int  GetRecordFileNumber(int owner);
	int  GetAlarmFileNumber(int owner);

	void BroadcaseInitFileMsg();
};
