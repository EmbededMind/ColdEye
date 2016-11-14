#pragma once

#include "Device\Camera.h"
#include "File\RecordFileButler.h"

// CSurfaceHolder

class CSurfaceHolder : public CDialogEx
{
	DECLARE_DYNAMIC(CSurfaceHolder)

public:
	CSurfaceHolder(CWnd* pParent = NULL);
	virtual ~CSurfaceHolder();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_HOLDER};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


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
	BOOL          mIsSelected;


private:
	WORD          m_wAlarmStamp;
	BOOL          ShouldWatch();

	CWnd          mSurface;





protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
};


