#pragma once


#include "Device\Camera.h"
#include "File\RecordFileButler.h"

#include "OSDPainter.h"

#include "FuckButton.h"

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

	void          ConnectRealPlay();         //��ʵʱ����������
	void          DisconnectRealPlay();      //�ر�ʵʱ����
	 

	void          StartRealPlay();           //����ʵʱ����������������ʵʱ��Ƶ
	void          StopRealPlay();            //ֹͣ����ʵʱ��Ƶ

	void          StartRecord(CFile* pf);    //��ʼ¼��
	void          StopRecord();              //ֹͣ¼��

	BOOL          StartAutoRecord();         //��ʼ�Զ����¼��
	void          StopAutoRecord();          //ֹͣ�Զ����¼��

	void          StartAlarmRecord(CFile* pFile);  //��ʼ¼�Ʊ�����Ƶ
	void          StopAlarmRecord();               //ֹͣ¼�Ʊ�����Ƶ

	BOOL          StartAutoWatch();                 //��ʼ�Զ�����
	void          StopAutoWatch();                  //ֹͣ�Զ�����

	void          PackageRecordFile();              //¼���ļ������

	void          OnAlarmTrigged();                 //����������Ӧ
	void          OnAlarmStop();                    //����������Ӧ

	void          OnDisconnect();                   //�豸������Ӧ
	void          OnReconnect();                    //�豸������Ӧ

	void          OnCameraLogOff();                 //ɾ�������

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRevsese();
	afx_msg void OnBnClickedDelete();
protected:
	afx_msg LRESULT OnUserMsgNofityKeydown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigOoChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigRdChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgCameraConfigAwChange(WPARAM wParam, LPARAM lParam);
};


