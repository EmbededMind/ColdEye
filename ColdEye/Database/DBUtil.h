#pragma once

class CDBUtil
{
private:
	CDBUtil();
	~CDBUtil();

public:
	static CDBUtil* GetInstance();

	int    CDBUtil::SimpleQuery(char* which_table, char* what);
	int    CDBUtil::SimpleQuery(char* which_table, char* statement, char* condition);
	BOOL   CDBUtil::SimpleUpdate(char* which_table, char* statement, char* condition,int val);
};
