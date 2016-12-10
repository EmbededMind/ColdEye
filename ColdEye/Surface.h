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

	void          ConnectRealPlay();         //打开实时播放数据流
	void          DisconnectRealPlay();      //关闭实时播放
	 
	void          SetOsdText(int xPos, int yPos, CString& text);
	void          SetOsdText(int xPos, int yPos, char* pText);

	void          StartRealPlay();           //解码实时播放数据流，播放实时视频
	void          StopRealPlay();            //停止播放实时视频

	void          StartRecord(CFile* pf);    //开始录像
	void          StopRecord();              //停止录像

	BOOL          StartAutoRecord();         //开始自动打包录像
	void          StopAutoRecord();          //停止自动打包录像

	void          StartAlarmRecord(CFile* pFile);  //开始录制报警视频
	void          StopAlarmRecord();               //停止录制报警视频


	void          StartWatch();
	BOOL          StopWatch();

	BOOL          StartAutoWatch();                 //开始自动看船
	void          StopAutoWatch();                  //停止自动看船

	void          PackageRecordFile();              //录像文件按打包

	void          OnAlarmTrigged();                 //报警触发响应
	void          OnAlarmStop();                    //报警结束响应

	void          OnDisconnect();                   //设备掉线响应
	void          OnReconnect();                    //设备重连响应

	void          OnCameraLogOff();                 //删除摄像机
	
	void          Delete();
	void          SetPos(CRect& r);


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

	CWnd*         pSaveParent;
	CRect         mSplitPos;
	void          ZoomOut();
	void          ZoomIn();


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNcPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
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


