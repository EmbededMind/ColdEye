#pragma once


class CDBLogger
{
private:
	CDBLogger();
	~CDBLogger();

public :
	static CDBLogger* GetInstance();
};