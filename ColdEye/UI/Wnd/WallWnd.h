#pragma once

#include "UIlib.h"
#include "Control\UISurface.h"


#include "Device\Camera.h"

#include <map>

typedef std::map<long, CCamera*> Device_Map;

using namespace DuiLib;

class CWallWnd : public CWindowWnd, public INotifyUI
{
public :
	CWallWnd();
	//CWallWnd(CWindowWnd* pDuiWnd);
	//CWallWnd(CWnd* pMfcWnd);
	//CWallWnd(HWND hWnd);

	~CWallWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void    Notify(TNotifyUI& msg);


	BOOL Invest(CCamera* pCamera);
	void DesignSurfaceLayout();
	void ExecuteSurfaceLayout();

	//void ReConnect(LONG loginId, char* szIp, LONG port);
	void OnDisConnect(LONG loginId, char* szIp, LONG port);
	//CCamera* FindCamera(long loginId);
	CSurfaceUI* FindSurface(long loginId);
	
protected:
	CPaintManagerUI m_PaintManager;
	CContainerUI*   m_pContainer;
	CSurfaceUI* mSurfaces[6];

	Device_Map mReconnectDevMap;


	DWORD       mRows;
	DWORD       mCols;

};