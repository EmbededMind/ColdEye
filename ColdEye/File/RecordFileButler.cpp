#include "stdafx.h"
#include "RecordFileButler.h"

CRecordFileButler::CRecordFileButler()
{}

CRecordFileButler::~CRecordFileButler()
{}

void CRecordFileButler::Notify(UINT opt, WPARAM wParam, LPARAM lParam)
{
	list<Observer*>::iterator iter = this->mObserveres.begin();

	for (; iter != mObserveres.end(); iter++) {
		(*iter)->Update(opt, this->m_FileType, lParam);
	}
}


CFile* CRecordFileButler::AllocRecordFile()
{
	CTime time = CTime::GetCurrentTime();
	CString fileName;
	fileName  = time.Format(_T("%Y%m%d%H%M%S"));

	CFileException fException;
	if (!m_File.Open(m_Direction + fileName, CFile::modeCreate | CFile::modeWrite, &fException) ){
		return NULL;
	}

	m_pFileInfo = new CRecordFileInfo();
	m_pFileInfo->tBegin = time.GetTime();

	Notify(FILE_OPT_ADD, m_FileType, (LPARAM)m_pFileInfo);
	
	return &m_File;
}


void CRecordFileButler::ReleaseRecordFile()
{
	CTime time = CTime::GetCurrentTime();

	if (m_pFileInfo != CFile::hFileNull) {
		CFileStatus  status;
		if (m_File.GetStatus(status)) {
			m_pFileInfo->dlSize = status.m_size;
		}

		if (mutex != 0) {
			Print("Mutex error:%d", mutex);
			ASSERT(false);
		}

		mutex = 2;

		m_File.Close();

		if (m_pFileInfo->dlSize == 0) {
			CFile::Remove(status.m_szFullName);
		}

		mutex = 0;

		this->Notify(FILE_OPT_END, m_FileType, (LPARAM)m_pFileInfo);
	}
	else {
PrintE("Release null handle file");
	}
}



void CRecordFileButler::SetDirection(CString& dir)
{
	this->m_Direction = dir;
}


void CRecordFileButler::SetFileType(RECORD_FILE_TYPE type)
{
	m_FileType = type;
}