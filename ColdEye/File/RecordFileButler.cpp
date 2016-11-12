#include "stdafx.h"
#include "RecordFileButler.h"

CRecordFileButler::CRecordFileButler()
{}

CRecordFileButler::~CRecordFileButler()
{}

//
//void CRecordFileButler::Notify(string state, CRecordFileInfo* pInfo)
//{
//	list<Observer*>::iterator iter = this->mObserveres.begin();
//
//	for (; iter != mObserveres.end(); iter++) {
//		(*iter)->Update(state, (DWORD_PTR)pInfo);
//	}
//}
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

		m_File.Close();

		if (m_pFileInfo->dlSize == 0) {
			CFile::Remove(status.m_szFullName);
		}

;
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


void CRecordFileButler::SetFileType(UINT type)
{
	m_FileType = type;
}