#pragma once
#include "Surface.h"

// CWallDlg �Ի���

class CWallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWallDlg)

public:
	CWallDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWallDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WALL };
#endif


public:
    bool      Invest(CPort* pPort);
	

	BOOL Invest(CCamera* pCamera);

	void Delete(CSurface* pSurface);

	CSurface* FindSurface(long loginId);

	void OnDisconnect(LONG loginId, char* szIp, LONG port);

	void DesignSurfaceLayout();
	void ExecuteSurfaceLayout();


protected:
	CSurface*        mSurfaces[6];

	DWORD            mRows;
	DWORD            mCols;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg LRESULT OnUserMsgLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgLogoff(WPARAM wParam, LPARAM lParam);
};
