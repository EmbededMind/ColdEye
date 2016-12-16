
// stdafx.cpp : 只包括标准包含文件的源文件
// ColdEye.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <Psapi.h>
SQLiteWrapper sqlite;

int mutex = 0;

void ShowMemoryInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	Print("内存使用:%0.2f MB", (double)pmc.PagefileUsage / 1024 / 1024);
}