#include "stdafx.h"

#include "Database\DBLogger.h"


CDBLogger::CDBLogger()
{
}


CDBLogger::~CDBLogger()
{}


CDBLogger* CDBLogger::GetInstance()
{
	static CDBLogger mInstance;
	return &mInstance;
}