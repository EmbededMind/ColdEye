﻿#pragma once

#include "Pattern\Subject.h"
#include "File\RecordFileInfo.h"


class CRecordFileButler:
	public Subject
{
public:
	CRecordFileButler();
	~CRecordFileButler();

	CFile* AllocRecordFile();
	void   ReleaseRecordFile();


	void   SetDirection(CString& dir);
    void   SetFileType(RECORD_FILE_TYPE fileType);
	void   SetOwner(UINT owner);


	virtual void Notify(UINT opt, WPARAM wParam, LPARAM lParam);

protected:
	CFile        m_File;
	CRecordFileInfo* m_pFileInfo;
	CString      m_Direction;
	UINT         m_FileType;
	UINT         m_Owner;
};

