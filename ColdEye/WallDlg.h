#pragma once
#include "Surface.h"

// CWallDlg 对话框

class CWallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWallDlg)

public:
	CWallDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWallDlg();


	std::list<CPort*> mReconnectPort;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WALL };
#endif


public:
    bool      Ingest(CPort* pPort);
	


	void Delete(CSurface* pSurface);

	CSurface* FindSurface(long loginId);
	CSurface* FindSurface(CPort* pPort);

	void OnDisconnect(LONG loginId, char* szIp, LONG port);

	void DesignSurfaceLayout();
	void ExecuteSurfaceLayout();

	void DeleteSurface(CSurface* pSurface);
protected:
	CSurface*        mSurfaces[6];

	DWORD            mRows;
	DWORD            mCols;


private:
	int              mMargin;
	int              mGrap;
	CRect            mSurfaceArea;
	int              mSurfaceWidth;
	int              mSurfaceHeight;


	void             TestIngestOne();
	void             TestSplitOne();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


protected:
	afx_msg LRESULT OnUserMsgLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgLogoff(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgRelogin(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUserMsgDisconnect(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUserMsgCameraConfigChange(WPARAM wParam, LPARAM lParam);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnPaint();
};
