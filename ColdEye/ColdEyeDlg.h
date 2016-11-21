
// ColdEyeDlg.h : 头文件
//

#pragma once

#include "WallDlg.h"
#include "Wnd\MyMenuWnd.h"
#include "Com\SerialPort.h"

// CColdEyeDlg 对话框
class CColdEyeDlg : public CDialogEx
{
	// 构造
public:
	CColdEyeDlg(CWnd* pParent = NULL);	// 标准构造函数

	CMyMenuWnd mMenu;
protected:
	CWallDlg  mWall;



	void UpdateLayout();

										// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLDEYE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


														// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
