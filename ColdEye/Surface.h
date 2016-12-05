#pragma once


//#include "Device\Camera.h"

#include "Device\Port.h"

#include "File\RecordFileButler.h"

#include "OSDPainter.h"

#include "FuckButton.h"

#include "H264Play.h"

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
	bool m_bIsAutoRecordEnabled;
	bool m_bIsAlarming;
	bool m_bIsWatching;
	bool m_bIsAutoWatchEnabled;

	CCamera* m_BindedCamera;

	CPort*   m_BindedPort;

	CRecordFileButler  m_RecordFileButler;
	CRecordFileButler  m_AlarmFileButler;

	COSDPainter   mOsdPainter;

	//void          BindCamera(CCamera* pCamera);

	void          BindPort(CPort* pPort);

	void          ExecuteConfig();
	void          ModifyConfig(DeviceConfig* pConfig);

	void          ConnectRealPlay();         //��ʵʱ����������
	void          DisconnectRealPlay();      //�ر�ʵʱ����
	 
	void          SetOsdText(int xPos, int yPos, CString& text);
	void          SetOsdText(int xPos, int yPos, char* pText);

	void          StartRealPlay();           //����ʵʱ����������������ʵʱ��Ƶ
	void          StopRealPlay();            //ֹͣ����ʵʱ��Ƶ

	void          StartRecord(CFile* pf);    //��ʼ¼��
	void          StopRecord();              //ֹͣ¼��

	BOOL          StartAutoRecord();         //��ʼ�Զ����¼��
	void          StopAutoRecord();          //ֹͣ�Զ����¼��

	void          StartAlarmRecord(CFile* pFile);  //��ʼ¼�Ʊ�����Ƶ
	void          StopAlarmRecord();               //ֹͣ¼�Ʊ�����Ƶ


	void          StartWatch();
	BOOL          StopWatch();

	BOOL          StartAutoWatch();                 //��ʼ�Զ�����
	void          StopAutoWatch();                  //ֹͣ�Զ�����

	void          PackageRecordFile();              //¼���ļ������

	void          OnAlarmTrigged();                 //����������Ӧ
	void          OnAlarmStop();                    //����������Ӧ

	void          OnDisconnect();                   //�豸������Ӧ
	void          OnReconnect();                    //�豸������Ӧ

	void          OnCameraLogOff();                 //ɾ�������
	
	void          Delete();

	void          SetPos(CRect& rPos);
	void          SetPos(int x, int y, int cx, int cy);
	void          SetSplitPosParam(CRect& rPos);


protected:
	//CWnd          mControlWnd;
	//CWnd          mSurface;

	//CButton       mDelBtn;
	//CButton       mReverseBtn;
	CFuckButton     mReverseBtn;
	CFuckButton     mDelBtn;


	BOOL          mHasFocused;
	BOOL          RegisterWindowClass(HINSTANCE hInstance = NULL);



private:
	OSD_INFO_TXT  m_OsdInfoText;

	long          m_hRealPlay;
	WORD          m_wAlarmStamp;
	BOOL          ShouldWatch();
	BOOL          ShouldWatch(CTime& refTime);
	UINT          GetNextWatchEventElapse(CTime& refTime);

	bool          mIsLargeMode;

	CRect         mSplitPos;
	void          ZoomOut();
	void          ZoomIn();

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRevsese();
	afx_msg void OnBnClickedDelete();
protected:
	afx_msg LRESULT OnUserMsgNofityKeydown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigOoChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigRdChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigAwChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigAwtime(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	afx_msg LRESULT OnUserMsgSystemConfig(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
};


