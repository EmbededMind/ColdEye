#include "stdafx.h"

#include "DBUtil.h"

CDBUtil::CDBUtil()
{

}


CDBUtil::~CDBUtil()
{

}


CDBUtil*  CDBUtil::GetInstance()
{
	static CDBUtil mInstance;
	return &mInstance;
}



int CDBUtil::SimpleQuery(char* which_table, char* what)
{
	char sqlStmt[64];

	sprintf_s(sqlStmt, "SELECT %s from FROM %s;", what, which_table);

	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);

	while (stmt->NextRow()) {
		return stmt->ValueInt(0);
	}

	return 0;
}



int CDBUtil::SimpleQuery(char* which_table, char* statement, char* condition)
{
	char sqlStmt[64];

	sprintf_s(sqlStmt, "SELECT %s from FROM %s WHERE %s;", statement, which_table, condition);

	SQLiteStatement* stmt = sqlite.Statement(sqlStmt);

	while (stmt->NextRow()) {
		return stmt->ValueInt(0);
	}

	return 0;
}






BOOL  CDBUtil::SimpleUpdate(char* which_table, char* statement, char* condition , int val)
{
	char sqlStmt[64];

	sprintf_s(sqlStmt, "UPDATE %s SET %s = %d WHERE %s;", which_table, statement, val, condition);

	return sqlite.DirectStatement(sqlStmt);
}