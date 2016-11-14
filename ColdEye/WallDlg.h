#pragma once
#include "Surface.h"

// CWallDlg 对话框

class CWallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWallDlg)

public:
	CWallDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWallDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WALL };
#endif


public:
	BOOL Invest(CCamera* pCamera);
	void OnDisconnect(LONG loginId, char* szIp, LONG port);

	void DesignSurfaceLayout();
	void ExecuteSurfaceLayout();


protected:
	//CSurfaceHolder*  mHolders[6];
	CSurface*        mSurfaces[6];

	DWORD            mRows;
	DWORD            mCols;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUserMsgLogin(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
