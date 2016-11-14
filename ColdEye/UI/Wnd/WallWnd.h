#pragma once

#include "UIlib.h"
//#include "Control\UISurface.h"

#include "SurfaceHolder.h"


#include "Device\Camera.h"



using namespace DuiLib;

class CWallWnd : public CWindowWnd, public INotifyUI
{
public :
	CWallWnd();


	~CWallWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void    Notify(TNotifyUI& msg);


	BOOL Invest(CCamera* pCamera);
	void DesignSurfaceLayout();
	void ExecuteSurfaceLayout();

	void OnDisConnect(LONG loginId, char* szIp, LONG port);
	//CSurfaceUI* FindSurface(long loginId);
	
protected:
	CPaintManagerUI m_PaintManager;
	CContainerUI*   m_pContainer;
	//CSurfaceUI* mSurfaces[6];

	
	CSurfaceHolder* mHolders[6];

	DWORD       mRows;
	DWORD       mCols;

};