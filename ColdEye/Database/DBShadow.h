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

	static CDBShadow* mInstance_1;
	SQLiteWrapper sqlite;

	void  CheckTables();

	void  AddFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);
	void  EndFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);
	void  DelFileInfo(list<CRecordFileInfo*>&, CRecordFileInfo*);

public:
	list<CRecordFileInfo*> mReocrdFileInfoList;
	list<CRecordFileInfo*> mAlarmFileInfoList;


	static CDBShadow* GetInstance();

	virtual void Update(UINT opt, WPARAM wParam, LPARAM lParam);

	void Init();
	void SynchronizeWithDB();			
};
