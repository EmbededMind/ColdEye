#pragma once

#include "Device\Camera.h"
#include "File\RecordFileButler.h"
//#include "File\AlarmFileManager.h"

#include "UIlib.h"


using namespace DuiLib;

class CSurfaceUI : public CControlUI
{
public:
	DECLARE_DUICONTROL(CSurfaceUI)
public:
	CSurfaceUI();
	CSurfaceUI(CWnd* pMFCWnd);
	~CSurfaceUI();


	long m_hRealPlay;
	LONG m_lPlayPort;

	CFile*      m_pAlmFile;
	CFile*      m_pRdFile;
	bool  m_bIsRecording;
	bool  m_bIsAlarming;
	bool  m_bIsWatching;


	CCamera*   m_BindedCamera;
	CRecordFileButler m_RecordFileButler;
	CRecordFileButler  m_AlarmFileButler;

	virtual LPCTSTR GetClass() const;
	virtual void    SetInternVisible(bool bVisible  = true);
	virtual LPVOID  GetInterface(LPCTSTR pstrName);
	virtual void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	

	virtual void    DoPaint(HDC hDC, const RECT& rcPaint);
	virtual void    PaintBorder(HDC hDC);
	virtual void    SetPos(RECT rc);

	void            SetFixedPos(RECT rc);


	void   SetBorderColor(DWORD dwColor);
	DWORD  GetBorderColor();
	void   SetFocusedBorderColor(DWORD dwColor);
	DWORD  GetFocusedBorderColor();

	BOOL Attach(HWND hWndNew);

	void BindCamera(CCamera* pCamera);

	void ExecuteLocalConfig();
	void ExecuteLocalConfig(LocalConfig* pConfig);

	HWND Detach();

	void DoEvent(TEventUI& event);

	void ConnectRealPlay();
	void DisconnectRealPlay();

	HWND GetWindowHandle();

	void StartRecord(CFile* pf);
	void StopRecord();

	BOOL StartAutoRecord();
	void StopAutoRecord();


	void StartAlarmRecord(CFile* pFile);
	void StopAlarmRecord();

	BOOL StartAutoWatch();
	void StopAutoWatch();

	void PackageReocrdFile();

	void OnAlarmTrigged();
	void OnAlarmStop();

	void OnDisConnect();
	void OnReConnect();

protected:
	HWND m_hWnd;

	UINT m_uSurfaceState;

	DWORD m_dwBorderColor;
	DWORD m_dwFocusedBorderColor;



private:

	WORD  m_wAlarmStamp;

	BOOL  ShouldWatch();	
};
