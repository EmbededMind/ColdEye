
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ColdEye.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include <Psapi.h>
SQLiteWrapper sqlite;

int mutex = 0;

void ShowMemoryInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	Print("�ڴ�ʹ��:%0.2f MB", (double)pmc.PagefileUsage / 1024 / 1024);
}