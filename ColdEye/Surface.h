#pragma once


#include "Device\Camera.h"
#include "File\RecordFileButler.h"

#include "OSDPainter.h"

// CSurface

class CSurface : public CWnd
{
	DECLARE_DYNAMIC(CSurface)

public:
	CSurface();
	virtual ~CSurface();

public:

	LONG m_lPlayPort;

	CFile*     m_pAlmFile;
	CFile*     m_pRdFile;
	
	bool m_bIsRealPlaying;
	bool m_bIsRecording;
	bool m_bIsAlarming;
	bool m_bIsWatching;

	CCamera* m_BindedCamera;
	CRecordFileButler  m_RecordFileButler;
	CRecordFileButler  m_AlarmFileButler;

	COSDPainter   mOsdPainter;

	void          BindCamera(CCamera* pCamera);

	void          ExecuteLocalConfig();
	void          ExecuteLocalConfig(LocalConfig* pConfig);

	void          ConnectRealPlay();
	void          DisconnectRealPlay();


	void          StartRealPlay();
	void          StopRealPlay();

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
	CWnd          mControlWnd;
	CWnd          mSurface;


	BOOL          mHasFocused;
	BOOL          RegisterWindowClass(HINSTANCE hInstance = NULL);




private:
	long          m_hRealPlay;
	WORD          m_wAlarmStamp;
	BOOL          ShouldWatch();


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNcPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnUserMsgRelogin(WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


