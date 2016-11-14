#pragma once


#include "Device\Camera.h"

// CSurface

class CSurface : public CWnd
{
	DECLARE_DYNAMIC(CSurface)

public:
	CSurface();
	virtual ~CSurface();

public:
	long m_hRealPlay;
	LONG m_lPlayPort;

	CFile*     m_pAlmFile;
	CFile*     m_pRdFile;

	bool m_bIsRecording;
	bool m_bIsAlarming;
	bool m_bIsWatching;

	CCamera* m_BindedCamera;
	CRecordFileButler  m_RecordFileButler;
	CRecordFileButler  m_AlarmFileButler;

	void          BindCamera(CCamera* pCamera);

	void          ExecuteLocalConfig();
	void          ExecuteLocalConfig(LocalConfig* pConfig);

	void          ConnectRealPlay();
	void          DisconnectRealPlay();

	void          StartRecord(CFile* pf);
	void          StopRecord();

	BOOL          StartAutoRecord();
	void          StopAutoRecord();

	void          StartAlarmRecord(CFile* pFile);
	void          StopAlarmRecord();

	BOOL          StartAutoWatch();
	void          StopAutoWatch();

	void          PackageRecordFile();

	void          OnAlarmTrigged();
	void          OnAlarmStop();

	void          OnDisconnect();
	void          OnReconnect();


protected:
	BOOL          mHasFocused;
	BOOL          RegisterWindowClass(HINSTANCE hInstance = NULL);


private:
	WORD          m_wAlarmStamp;
	BOOL          ShouldWatch();


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNcPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


