#include "stdafx.h"
#include "ExHardDrive.h"
#include "Wnd\MyMenuWnd.h"
#include "ColdEyeDlg.h"

CExHardDrive::CExHardDrive()
{
}
CExHardDrive::~CExHardDrive()
{
	SetEvent(mEndEvent);
}
BOOL CExHardDrive::Init(CWnd * pOwner, CString diskname)
{
	assert(pOwner != NULL);
	if (mIsThreadAlive)
	{
		do
		{
			SetEvent(mEndEvent);
		} while (mIsThreadAlive);
	}
	if (mEndEvent != NULL)
		ResetEvent(mEndEvent);
	else
		mEndEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mUpdataEvent != NULL)
		ResetEvent(mUpdataEvent);
	else
		mUpdataEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mCopyEvent != NULL)
		ResetEvent(mCopyEvent);
	else
		mCopyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (mScanEvent != NULL)
		ResetEvent(mScanEvent);
	else
		mScanEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	mHEventArray[0] = mEndEvent;
	mHEventArray[1] = mScanEvent;
	mHEventArray[2] = mCopyEvent;
	mHEventArray[3] = mUpdataEvent;

	mDiskName = diskname;
	mCOwner = pOwner;
	mHOwner = static_cast<CColdEyeDlg*>(mCOwner)->GetMyMenu().GetHWND();
	mIsInsert = TRUE;
	mCancelCopy = false;
	CheckCopyFolder();
	return TRUE;
}

UINT CExHardDrive::ExHardDriveThread(LPVOID pParam)
{
	CExHardDrive *ExHardDrive = (CExHardDrive*)pParam;

	ExHardDrive->mIsThreadAlive = TRUE;
	DWORD Event = 0;
	for (;;)
	{
		Event = WaitForMultipleObjects(4, ExHardDrive->mHEventArray, FALSE, INFINITE);
		switch (Event)
		{
		case 0:
			printf("m_endEvent\n");
			ExHardDrive->mIsThreadAlive = FALSE;
			ResetEvent(ExHardDrive->mEndEvent);
			AfxEndThread(100);
			break;
		case 1:
			if (!ExHardDrive->FindUpdataFile(ExHardDrive->mUpDateFilePath))
			{
				ExHardDrive->mUpDateFilePath = _T("NULL");
			}
			ResetEvent(ExHardDrive->mScanEvent);
			break;
		case 2:
		{
			Print("before CopyFileEx");
			BOOL bSucceed = CopyFileEx(ExHardDrive->mCopyFromPath, ExHardDrive->mCopyToPath,
				(LPPROGRESS_ROUTINE)CopyProgressCall,
				ExHardDrive, NULL,
				COPY_FILE_ALLOW_DECRYPTED_DESTINATION | COPY_FILE_COPY_SYMLINK | COPY_FILE_FAIL_IF_EXISTS);
			Print("after CopyFileEx");
			if (ExHardDrive->mHOwner != NULL)
			{
				if (bSucceed)
				{
					printf("copy succeed!\n");
					//发送结束， 成功 消息号是 COPY_END 数据是 COPY_END_SUCCEED
					::PostMessage(static_cast<CColdEyeDlg*>(ExHardDrive->mCOwner)->mMessageBox->GetHWND(), USER_MSG_COPY_STOP, STATUS_COPY_SUCCEED, (LPARAM)ExHardDrive->mFileInfo);
				}
				else
				{
					printf("copy fail! err:%d\n",GetLastError());
					if (static_cast<CColdEyeDlg*>(ExHardDrive->mCOwner)->mMessageBox)
					{
						if (GetLastError() == 2) {
							Print("U OUT");
							//PathFileExists
							if (!PathIsDirectory(ExHardDrive->mDiskName))
							{
								::PostMessage(static_cast<CColdEyeDlg*>(ExHardDrive->mCOwner)->mMessageBox->GetHWND(), USER_MSG_EXHARDDRIVE_OUT, NULL, NULL);
								ResetEvent(ExHardDrive->mCopyEvent);
								break;
							}
						}
						Print("File Name:%S", ExHardDrive->mCopyFromPath);
						//发送结束， 失败 消息号是 COPY_END 数据是 COPY_END_FAIL
						::PostMessage(static_cast<CColdEyeDlg*>(ExHardDrive->mCOwner)->mMessageBox->GetHWND(), USER_MSG_COPY_STOP, STATUS_COPY_FIAL, (LPARAM)ExHardDrive->mFileInfo);
					}
				}
			}
			ResetEvent(ExHardDrive->mCopyEvent);
		}
			break;
		case 3:
			ExHardDrive->FindUpdataFile(ExHardDrive->mDiskName);
			ResetEvent(ExHardDrive->mUpdataEvent);
			break;
		default:
			break;
		}
	}
	return 0;
}

BOOL CExHardDrive::FindUpdataFile(CString path)
{
	CFileFind fileFinder;
	CString filePath = path + _T("*.*");

	BOOL bFinished = fileFinder.FindFile(filePath);
	while (bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() && !fileFinder.IsDots())
		{
			CString fileName = fileFinder.GetFileName();
			if (fileName == _T("haha"))
			{
				mUpDateFilePath = fileFinder.GetFilePath();
				return TRUE;
			}
			else
			{
				FindUpdataFile(fileFinder.GetFilePath() + _T("\\"));
			}
		}
		else
		{
			CString fileName = fileFinder.GetFileName();
			if (fileName == _T("hehe.txt"))
			{
				mUpDateFilePath = fileFinder.GetFilePath();
				return TRUE;
			}
		}
	}
	fileFinder.Close();
	return 0;
}

BOOL CExHardDrive::RecordFilePath(CString path, CString FileName)
{
	mCopyFromPath = path;
	if (mRecordType)
	{
		mCopyToPath = mDiskName + _T("AlarmRecord\\");
	}
	else
	{
		mCopyToPath = mDiskName + _T("NormalRecord\\");
	}
	CString tmp;
	tmp.Format(_T("%d\\"), mFileInfo->nOwner);
	mCopyToPath += tmp;
	mCopyToPath += FileName;
	Print("CopyFile path : %S", mCopyToPath);
	SetEvent(mCopyEvent);
	return 0;
}

DWORD CExHardDrive::CopyProgressCall(LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID lpData)
{
	Print("copy callback ---------");
	if (((CExHardDrive*)lpData)->mCancelCopy)
	{
		((CExHardDrive*)lpData)->mCancelCopy = FALSE;
		return PROGRESS_CANCEL;
	}
	if (dwCallbackReason == CALLBACK_STREAM_SWITCH)
	{
		//发送开始文件的录制信息 , 消息号是 COPY_START
	}
	else
	{
		((CExHardDrive*)lpData)->mTotalFileSize = TotalFileSize.QuadPart;
		((CExHardDrive*)lpData)->mTotalBytesTransferred = TotalBytesTransferred.QuadPart;
		/*printf("FILE COPY INFO : %lld, %lld\n", ((CExHardDrive*)lpData)->mTotalFileSize, ((CExHardDrive*)lpData)->mTotalBytesTransferred);*/
		//发送文件复制中的信息，mTotalFileSize是文件的总大小， mTotalBytesTransferred是文件已经复制的大小, 消息号是 COPY_INFO
		::SendMessage(static_cast<CColdEyeDlg*>(((CExHardDrive*)lpData)->mCOwner)->mMessageBox->GetHWND(), USER_MSG_COPY_INFO, TotalBytesTransferred.QuadPart, (LPARAM)((CExHardDrive*)lpData)->mFileInfo);
	}
	return PROGRESS_CONTINUE;
}

BOOL CExHardDrive::StartMonitoring()
{
	if (!mIsInsert) return 0;
	if (!(mThread = AfxBeginThread(ExHardDriveThread, this)))
		return FALSE;
	printf("Thread Start\n");
	return TRUE;
}

BOOL CExHardDrive::EndThread()
{
	mIsInsert = FALSE;
	SetEvent(mEndEvent);
	return 0;
}

BOOL CExHardDrive::GetStatus(USBFlashDiskStatus *pStaus)
{
	if (!mIsInsert) return 0;
	ULARGE_INTEGER   uiFreeBytesAvailableToCaller;
	ULARGE_INTEGER   uiTotalNumberOfBytes;
	ULARGE_INTEGER   uiTotalNumberOfFreeBytes;
	TRACE("m_scanEvent\n");
	if (GetDiskFreeSpaceExW(mDiskName,
		&uiFreeBytesAvailableToCaller,
		&uiTotalNumberOfBytes,
		&uiTotalNumberOfFreeBytes))
	{
		pStaus->mSpaceLeft = (FLOAT)((LONGLONG)uiTotalNumberOfFreeBytes.QuadPart);
		pStaus->mSpacetotal = (FLOAT)((LONGLONG)uiTotalNumberOfBytes.QuadPart);
		TRACE(_T("--%f-- %f"), pStaus->mSpaceLeft, pStaus->mSpacetotal);
	}
	return TRUE;
}

BOOL CExHardDrive::Updata()
{
	if (!mIsInsert || !mIsThreadAlive) return 0;
	SetEvent(mUpdataEvent);
	return TRUE;
}

BOOL CExHardDrive::CopyRecord(CRecordFileInfo *FileInfo, UINT FileType)
{
	if (!mIsInsert || !mIsThreadAlive) return 0;
	mFileInfo = FileInfo;
	mRecordType = FileType;
	CTime time = FileInfo->tBegin;
	CString filename, filepath, owner;
	filename.Format(_T("%d%02d%02d%02d%02d%02d.h264"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	owner.Format(_T("%1d\\"), FileInfo->nOwner);
	if (FileType)//1表示报警
	{
		filepath = _T(ALARM_RECORD_PATH) + owner + filename;
	}
	else
	{
		filepath = _T(NORMAL_RECORD_PATH) + owner + filename;
	}
	this->RecordFilePath(filepath, filename);
	return TRUE;
}
 
BOOL CExHardDrive::IsInsert()
{
	return this->mIsInsert;
}

BOOL CExHardDrive::FindUpdataPath()
{
	if (!mIsInsert || !mIsThreadAlive) return 0;
	SetEvent(mUpdataEvent);
	return 0;
}

void CExHardDrive::CancelCopy()
{
	mCancelCopy = true;
}

BOOL CExHardDrive::ScanDisk(CWnd *pOwner)
{
	DWORD mask;
	char tch[50];
	TCHAR diskname[50];
	CString DiskName;
	UINT type;
	mask = GetLogicalDrives();

	_itoa_s(mask, tch, 2);

	for (int i = 0; i < 32; i++)
	{
		if ((mask >> i) == 1)
		{
			diskname[0] = 'A' + i;
			diskname[1] = '\0';
			wcscat_s(diskname, TEXT(":\\"));
			DiskName.Format(_T("%s"), diskname);
			type = GetDriveType(DiskName);
			if (type == DRIVE_REMOVABLE)
			{
				this->Init(pOwner, DiskName);
				this->StartMonitoring();
				return true;
			}
		}
	}
	return false;
}

void CExHardDrive::CheckCopyFolder()
{
	if (!PathIsDirectory(mDiskName + _T("NormalRecord\\")))
	{
		CreateDirectory(mDiskName + _T("NormalRecord\\"), NULL);
	}
	if (!PathIsDirectory(mDiskName + _T("AlarmRecord\\")))
	{
		CreateDirectory(mDiskName + _T("AlarmRecord\\"), NULL);
	}
	CString tmp;
	for (int i = 1; i < 7; i++)
	{
		tmp.Format(_T("%d\\"), i);
		if (!PathIsDirectory(mDiskName + _T("NormalRecord\\") + tmp))
		{
			CreateDirectory(mDiskName + _T("NormalRecord\\") + tmp, NULL);
		}
		if (!PathIsDirectory(mDiskName + _T("AlarmRecord\\") + tmp))
		{
			CreateDirectory(mDiskName + _T("AlarmRecord\\") + tmp, NULL);
		}
	}
}

