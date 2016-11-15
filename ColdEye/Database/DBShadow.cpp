#include "stdafx.h"

#include "Database\DBShadow.h"
#include "Pattern\MsgSquare.h"

CRITICAL_SECTION g_cs;
CDBShadow::CDBShadow()
{
	bool bRet  = sqlite.Open("cold_eye.db");
	if (bRet == false) {
		Print("Database open failed");
		ASSERT(FALSE);
	}

	CheckTables();
}


CDBShadow::~CDBShadow()
{}


CDBShadow* CDBShadow::GetInstance()
{
	static CDBShadow mInstance;
	return &mInstance;
}


void CDBShadow::Update(UINT opt, WPARAM wParam, LPARAM lParam)
{
	std::list<CRecordFileInfo*>& infoList = (wParam == 1 ? mReocrdFileInfoList : mAlarmFileInfoList);
	MSG msg;
	EnterCriticalSection(&g_cs);
	switch (opt) 
	{
		case FILE_OPT_ADD:
			AddFileInfo(infoList,  (CRecordFileInfo*)lParam);
			break;
		//-----------------------------------------------
		case FILE_OPT_END:
			EndFileInfo(infoList, (CRecordFileInfo*)lParam);

			msg.message = USER_MSG_ADDFILE;
			msg.wParam = wParam;
			msg.lParam = lParam;
			CMsgSquare::GetInstance()->Broadcast(msg);
			break;
		//------------------------------------------------
		case FILE_OPT_DEL:
			DelFileInfo(infoList, (CRecordFileInfo*)lParam);

			msg.message = USER_MSG_DELFILE;
			msg.wParam = wParam;
			msg.lParam = lParam;
			CMsgSquare::GetInstance()->Broadcast(msg);
			break;
	}
	LeaveCriticalSection(&g_cs);
}


/**@brief 初始化数据库Shadow
 *
 * @note  由于此方法过程中会向窗口发送消息，所以必须在窗口创建完成后执行。
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





/**@biref 录像文件信息与数据库中的记录同步
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
		pInfo->bIsLocked = stmt->ValueInt(DB_COL_STATUS);
		pInfo->bIsOccupied = false;

		if (pInfo->tEnd > pInfo->tBegin) {
			mReocrdFileInfoList.push_back(pInfo);
		}
		else {
			Print("Invali record information");
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
		pInfo->bIsLocked = stmt->ValueInt(DB_COL_STATUS);
		pInfo->bIsOccupied = false;

		if (pInfo->tEnd > pInfo->tBegin) {
			mAlarmFileInfoList.push_back(pInfo);
		}
		else {
			Print("Invali record information");
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