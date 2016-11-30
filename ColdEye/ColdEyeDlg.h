
// ColdEyeDlg.h : 头文件
//

#pragma once

#include "WallDlg.h"
#include "Wnd\MyMenuWnd.h"
#include "Com\SerialPort.h"
#include "Wnd\MsgWnd.h"

class CMsgWnd;

// CColdEyeDlg 对话框
class CColdEyeDlg : public CDialogEx
{
	// 构造
public:
	CColdEyeDlg(CWnd* pParent = NULL);	// 标准构造函数

	CMsgWnd	*mMessageBox; 

protected:
	CWallDlg  mWall;
	CMyMenuWnd mMenu;

	void UpdateLayout();


private:
	CFont    m_TipFont;

	CRect    m_rTitle;
	POINT	 m_pTitleBk_left[4];
	POINT	 m_pTitleBk_center[4];
	POINT	 m_pTitleBk_right[4];

	CTime    m_SysTime;
	CRect    m_rSysTimeText;

	CString  m_AwTipText;
	CRect    m_rAwTipText;

	CString  m_HwTipText;
	CRect    m_rHwTipText;

	CBitmap  m_bmpUFlash;
	CRect    m_rFlash;

	std::list<uint8_t> mPendMacPort;
	std::list<CPort*>  mSearchPort;

	bool isFlashDisk;

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
	afx_msg LRESULT OnRecordVoice(WPARAM wParm, LPARAM lParm);
	CMyMenuWnd& GetMyMenu();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void PaintTitle(CPaintDC*);
protected:
	afx_msg LRESULT OnUserMsgCameraConfigChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMsgStopAlarm(WPARAM wParam, LPARAM lParam);
};
