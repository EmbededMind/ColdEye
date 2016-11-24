#pragma once
#include "stdafx.h"
#include "stdint.h"
#include "File\RecordFileInfo.h"

#define COPY_END_SUCCEED   1
#define COPY_END_FAIL      0

typedef struct USBFlashDiskStatus
{
	uint64_t mSpaceLeft;//字节
	uint64_t mSpacetotal;
}USBFlashDiskStatus;

class CExHardDrive
{
	~CExHardDrive();
private:
	CExHardDrive();
	USBFlashDiskStatus* mPStatus;
	bool mIsInsert;
	CWnd *mCOwner;//复制文件时用到
	HWND mHOwner;
	CString mDiskName;
	CString mUpDateFilePath;
	CString mCopyFromPath;
	CString mCopyToPath;

	HANDLE mEndEvent;
	HANDLE mUpdataEvent;
	HANDLE mCopyEvent;
	HANDLE mScanEvent;
	HANDLE mHEventArray[4];

	UINT mIsThreadAlive; //线程有没有启动
	CWinThread* mThread;

	ULONGLONG mTotalFileSize;
	ULONGLONG mTotalBytesTransferred;



	static UINT ExHardDriveThread(LPVOID pParam);
	BOOL FindUpdataFile(CString path);
	BOOL RecordFilePath(CString path, CString FileName);

	static DWORD CALLBACK CopyProgressCall(LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID lpData);
public:
	static CExHardDrive* GetInstance()
	{
		static CExHardDrive ins;
		return &ins;
	}
	BOOL Init(CWnd *pOwner, CString diskname);

/**************************/
	BOOL StartMonitoring();
	/***********/
	BOOL EndThread();
	BOOL GetStatus(USBFlashDiskStatus *pStaus);
	BOOL Updata();
	BOOL CopyRecord(CRecordFileInfo *FileInfo, UINT FileType);
	BOOL IsInsert();
	/******************/
	BOOL ScanDisk(CWnd *pOwner);
};

