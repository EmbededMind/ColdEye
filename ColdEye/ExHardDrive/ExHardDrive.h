#pragma once
#include "stdafx.h"
#include "stdint.h"
#include "File\RecordFileInfo.h"

#define COPY_START         WM_USER+1
#define COPY_INFO          WM_USER+2
#define COPY_END           WM_USER+3

#define COPY_END_SUCCEED   1
#define COPY_END_FAIL      0

typedef struct USBFlashDiskStatus
{
	uint64_t mSpaceLeft;//�ֽ�
	uint64_t mSpacetotal;
}USBFlashDiskStatus;

class CExHardDrive
{
	~CExHardDrive();
private:
	CExHardDrive();
	USBFlashDiskStatus* mPStatus;
	bool mIsInsert;
	CWnd *mCOwner;//�����ļ�ʱ�õ�
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

	UINT mIsThreadAlive; //�߳���û������
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
	/******************/
	BOOL ScanDisk(CWnd *pOwner);
};

