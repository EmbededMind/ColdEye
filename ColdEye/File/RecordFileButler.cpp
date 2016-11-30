#include "stdafx.h"
#include "RecordFileButler.h"
#include "File\RecordFileMetabolism.h"

extern CMutex mutex_RealData;


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
Print("Alloc file");
	CRecordFileMetabolism::GetInstance()->FileMetabolism();
	CTime time = CTime::GetCurrentTime();
	CString fileName;

	fileName  = time.Format(_T("%Y%m%d%H%M%S"));

	fileName  = m_Direction + fileName + _T(".h264");

if (m_FileType == 2)
	Print("Alloc file %S", fileName);
	CFileException fException;
	if (!m_File.Open(fileName, CFile::modeCreate | CFile::modeWrite, &fException) ){
		return NULL;
	}


	m_pFileInfo = new CRecordFileInfo();

	m_pFileInfo->nOwner = m_Owner;
	m_pFileInfo->tBegin = time.GetTime();
	

	Notify(FILE_OPT_ADD, m_FileType, (LPARAM)m_pFileInfo);

	return &m_File;
}


void CRecordFileButler::ReleaseRecordFile()
{
	Print("Release file");
	CTime time = CTime::GetCurrentTime();

	if (m_File != CFile::hFileNull) {
		CFileStatus  status;

mutex_RealData.Lock();

		if (m_File.GetStatus(status)) {
			m_pFileInfo->dlSize = status.m_size;
		}

		if (mutex != 0) {
			Print("Mutex error:%d", mutex);
			ASSERT(false);
		}

		mutex = 2;

		m_File.Close();

		//if (m_pFileInfo->dlSize == 0) {
		//	CFile::Remove(status.m_szFullName);
		//}

		mutex = 0;

		mutex_RealData.Unlock();

		m_pFileInfo->tEnd = time.GetTime();

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



void CRecordFileButler::SetOwner(UINT owner)
{
	m_Owner = owner;
}