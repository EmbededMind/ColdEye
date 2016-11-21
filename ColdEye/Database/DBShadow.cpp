#include "stdafx.h"

#include "Database\DBShadow.h"
#include "Pattern\MsgSquare.h"

CDBShadow::CDBShadow()
{
	bool bRet  = sqlite.Open("cold_eye.db");
	if (bRet == false) {
		Print("Database open failed");
		ASSERT(FALSE);
	}
	InitializeCriticalSection(&g_cs);
	CheckTables();
}


CDBShadow::~CDBShadow()
{
	DeleteCriticalSection(&g_cs);
}


CDBShadow* CDBShadow::GetInstance()
{
	static CDBShadow mInstance;
	return &mInstance;
}


void CDBShadow::Update(UINT opt, WPARAM wParam, LPARAM lParam)
{
	std::list<CRecordFileInfo*>& infoList = (wParam == 1 ? mReocrdFileInfoList : mAlarmFileInfoList);

	char sqlStmt[128];
	CRecordFileInfo* pInfo = (CRecordFileInfo*)lParam;

	MSG msg;  
	EnterCriticalSection(&g_cs);
	switch (opt)
	{
	case FILE_OPT_ADD:
		AddFileInfo(infoList, pInfo);
		sprintf_s(sqlStmt, "INSERT INTO %s (owner, begin_sec, end_sec) VALUES(%d,%I64d, 0);", wParam == RECORD_ALARM ? "alarm_record" : "normal_record",
			pInfo->nOwner, pInfo->tBegin);
		if (!sqlite.DirectStatement(sqlStmt)) {
			Print("Sql error:%s", sqlStmt);
		}
		break;
		//-----------------------------------------------
	case FILE_OPT_END:
		EndFileInfo(infoList, pInfo);

		sprintf_s(sqlStmt, "UPDATE %s SET end_sec = %I64d, size = %lld, status = 0 WHERE owner = %d AND begin_sec = %I64d;",
			wParam == RECORD_ALARM?"alarm_record":"normal_record",pInfo->tEnd, pInfo->dlSize, pInfo->nOwner, pInfo->tBegin);
		if (!sqlite.DirectStatement(sqlStmt)) {
			Print("Sql error:%s", sqlStmt);
		}

		msg.message = USER_MSG_ADDFILE;
		msg.wParam = wParam;
		msg.lParam = lParam;
		CMsgSquare::GetInstance()->Broadcast(msg);
		break;
		//------------------------------------------------
		case FILE_OPT_DEL:
			DelFileInfo(infoList, (CRecordFileInfo*)lParam);

			sprintf_s(sqlStmt, "DELETE FROM %s WHERE owner = %d begin_sec = %I64d;", 
				wParam == RECORD_ALARM ? "alarm_record" : "normal_record", pInfo->nOwner, pInfo->tBegin);
			if (sqlite.DirectStatement(sqlStmt)) {
				Print("Sql error:%s", sqlStmt);
			}

			msg.message = USER_MSG_DELFILE;
			msg.wParam = wParam;
			msg.lParam = lParam;
			CMsgSquare::GetInstance()->Broadcast(msg);
			break;
	}
	LeaveCriticalSection(&g_cs);
}


/**@brief ��ʼ�����ݿ�Shadow
 *
 * @note  ���ڴ˷��������л��򴰿ڷ�����Ϣ�����Ա����ڴ��ڴ�����ɺ�ִ�С�
 */
void CDBShadow::Init()
{

	SynchronizeWithDB();

	CMsgSquare* pSquare = CMsgSquare::GetInstance();
	MSG msg;
	msg.wParam = RECORD_NORMAl;
	msg.message = USER_MSG_INITFILE;	
	msg.lParam = (LPARAM) (&mReocrdFileInfoList);
	pSquare->Broadcast(msg);

	msg.wParam = RECORD_ALARM;
	msg.lParam = (LPARAM)(&mAlarmFileInfoList);
	pSquare->Broadcast(msg);
}





/**@biref ¼���ļ���Ϣ�����ݿ��еļ�¼ͬ��
 *
 */
void CDBShadow::SynchronizeWithDB()
{
	CRecordFileInfo* pInfo;

	char sqlStmt[128];
	sprintf_s(sqlStmt, "SELECT * FROM normal_record;");
	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);
	while (stmt->NextRow()) {
		pInfo = new CRecordFileInfo();

		pInfo->nOwner = stmt->ValueInt(DB_COL_OWNER);
		pInfo->tBegin = stmt->ValueInt(DB_COL_BEGIN_SEC);
		pInfo->tEnd = stmt->ValueInt(DB_COL_END_SEC);
		pInfo->dlSize = stmt->ValueInt(DB_COL_SIZE);
		pInfo->status = (RECORD_FILE_STATUS)stmt->ValueInt(DB_COL_STATUS);
		pInfo->bIsOccupied = false;

		if (pInfo->tEnd > pInfo->tBegin) {
			mReocrdFileInfoList.push_back(pInfo);
		}
		else {
			Print("Invalid record information");
			
			CTime t(pInfo->tBegin);
			CString filename;
			filename.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), pInfo->nOwner,
				t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

			filename = _T(NORMAL_RECORD_PATH) + filename;

			CFile::Remove(filename);
			sprintf_s(sqlStmt, "DELETE FROM normal_record WHERE owner = %d AND begin_sec = %I64d;",pInfo->nOwner, pInfo->tBegin);
			if (!sqlite.DirectStatement(sqlStmt)) {
				Print("Sql error:%s", sqlStmt);
			}
		}
	}

	sprintf_s(sqlStmt, "SELECT * FROM alarm_record;");
	stmt = sqlite.Statement(sqlStmt);
	while (stmt->NextRow()) {
		pInfo = new CRecordFileInfo();

		pInfo->nOwner = stmt->ValueInt(DB_COL_OWNER);
		pInfo->tBegin = stmt->ValueInt(DB_COL_BEGIN_SEC);
		pInfo->tEnd = stmt->ValueInt(DB_COL_END_SEC);
		pInfo->dlSize = stmt->ValueInt(DB_COL_SIZE);
		pInfo->status = (RECORD_FILE_STATUS)stmt->ValueInt(DB_COL_STATUS);
		pInfo->bIsOccupied = false;

		if (pInfo->tEnd > pInfo->tBegin) {
			mAlarmFileInfoList.push_back(pInfo);
		}
		else {
			Print("Invalid alarm record information");
			CTime t(pInfo->tBegin);
			CString filename;
			filename.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), pInfo->nOwner,
				t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

			filename = _T(ALARM_RECORD_PATH) + filename;

			CFile::Remove(filename);
			sprintf_s(sqlStmt, "DELETE FROM alarm_record WHERE owner = %d AND begin_sec = %I64d;", pInfo->nOwner, pInfo->tBegin);
			if (!sqlite.DirectStatement(sqlStmt)) {
				Print("Sql error:%s", sqlStmt);
			}
		}
	}
}



void CDBShadow::CheckTables()
{
	SQLiteStatement* stmt = sqlite.Statement("SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name='normal_reocrd';");
	if (!stmt->NextRow()) {
		Print("TABLE normal_record not exist");

		if (!sqlite.DirectStatement("CREATE TABLE normal_record(owner int not null,begin_sec int, end_sec int, size int, status int);")) {
			Print("Create table normal_record failed");
			ASSERT(FALSE);
		}
	}

	stmt = sqlite.Statement("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='alarm_record';");
	if (!stmt->NextRow()) {
		Print("TABLE alarm_record not exist");

		if (!sqlite.DirectStatement("CREATE TABLE normal_record(owner int not null,begin_sec int, end_sec int, size int, status int);")) {
			Print("Create table normal_record failed");
			ASSERT(FALSE);
		}
	}
}



void CDBShadow::AddFileInfo(list<CRecordFileInfo*>& infoList, CRecordFileInfo* pInfo)
{
	infoList.push_back(pInfo);
	Print("Add file");
}

void CDBShadow::EndFileInfo(list<CRecordFileInfo*>& infoList, CRecordFileInfo* pInfo)
{
	Print("End file");
}

void CDBShadow::DelFileInfo(list<CRecordFileInfo*>& infoList, CRecordFileInfo* pInfo)
{
	Print("Del file");
	list<CRecordFileInfo*>::iterator iter = infoList.begin();

	for (; iter != infoList.end(); iter++) {
		if ((*iter) == pInfo) {
			delete *iter;
			infoList.erase(iter);
			break;
		}
	}
}