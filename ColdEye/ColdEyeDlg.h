
// ColdEyeDlg.h : ͷ�ļ�
//

#pragma once

#include "WallDlg.h"
#include "Wnd\MyMenuWnd.h"
#include "Com\SerialPort.h"

// CColdEyeDlg �Ի���
class CColdEyeDlg : public CDialogEx
{
	// ����
public:
	CColdEyeDlg(CWnd* pParent = NULL);	// ��׼���캯��

	CMyMenuWnd mMenu;
protected:
	CWallDlg  mWall;



	void UpdateLayout();

										// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLDEYE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


														// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUserMsgScanDev(WPARAM wParam, LPARAM lParam);
	BOOL SetVolumeLevel(int type);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LONG OnCommReceive(WPARAM pData, LPARAM port);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

	CMyMenuWnd& GetMyMenu();
};
