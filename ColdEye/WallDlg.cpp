// WallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "WallDlg.h"
#include "afxdialogex.h"


bool __stdcall _cbDVRMessage(long loginId, char* pBuf, unsigned long bufLen, long dwUser)
{
	Print("Alarm message");

	CWallDlg* pWall = (CWallDlg*)dwUser;

	ASSERT(pWall != NULL);

	return 0;
}


void __stdcall _cbDisconnect(LONG loginId, char* szDVRIP, LONG DVRPort, DWORD userData)
{
	Print("------------------------------Disconnect---------------------------%s", szDVRIP);
	CWallDlg* pThis = (CWallDlg*)userData;
	ASSERT(pThis != nullptr);

	pThis->OnDisconnect(loginId, szDVRIP, DVRPort);
}


// CWallDlg 对话框

IMPLEMENT_DYNAMIC(CWallDlg, CDialogEx)

CWallDlg::CWallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CWallDlg::~CWallDlg()
{
}



BOOL CWallDlg::Invest(CCamera* pCamera)
{
	pCamera->LoadLocalConfig();

	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] == NULL) {
			pCamera->m_Id = i + 1;

			mSurfaces[i] = new CSurface();
			mSurfaces[i]->Create(NULL, _T("Surface"), WS_CHILD|WS_VISIBLE|WS_BORDER|WS_THICKFRAME, CRect(0,0,0,0), this, pCamera->m_Id);
			mSurfaces[i]->ShowWindow(SW_SHOW);


			mSurfaces[i]->BindCamera(pCamera);
			mSurfaces[i]->ExecuteLocalConfig();

			DesignSurfaceLayout();
			ExecuteSurfaceLayout();
			return TRUE;
		}
	}

	return FALSE;
}



void CWallDlg::DesignSurfaceLayout()
{
	int i = 0;
	int iSurfaceNumber = 0;

	for (; i < 6; i++) {
		if (mSurfaces[i] != NULL) {
			iSurfaceNumber++;
		}
	}

	if (iSurfaceNumber == 0) {
		mRows = 0;
		mCols = 0;
		return;
	}

	int qVal = 0;
	for (i = 1; ; i++) {
		qVal = i * 1;
		if (qVal >= iSurfaceNumber) {
			mCols = i;
			break;
		}
	}

	mRows = iSurfaceNumber / mCols + (iSurfaceNumber%mCols) > 0 ? 1 : 0;
}


void CWallDlg::ExecuteSurfaceLayout()
{
	CRect rClient;
	GetClientRect(rClient);

	long nWidth = rClient.Width() / mCols;
	long nHeight = nWidth * 8 / 16;
	long nOrgX = (rClient.Width() - nWidth*mCols) / 2;
	long nOrgY = (rClient.Height() - nHeight* mRows) / 2;

	int cnt = 0;
	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] != NULL) {
			long xPos = nOrgX = (cnt % mCols) * nWidth;
			long yPos = nOrgY = (cnt / mCols)*nHeight;

			mSurfaces[i]->SetWindowPos(NULL, xPos, yPos, nWidth, nHeight, 0);
			cnt++;
		}
	}
}


void CWallDlg::OnDisconnect(LONG loginId, char* szIp, LONG port)
{

}


void CWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWallDlg, CDialogEx)
	ON_MESSAGE(USER_MSG_LOGIN, &CWallDlg::OnUserMsgLogin)
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CWallDlg 消息处理程序


BOOL CWallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	H264_DVR_Init(_cbDisconnect, (DWORD)this);
	H264_DVR_SetDVRMessCallBack(_cbDVRMessage, (long)this);

	PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CWallDlg::OnUserMsgLogin(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		Print("Login...");
		Invest( (CCamera*)lParam);
	}

	return 0;
}


void CWallDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
}


void CWallDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	Print("Wall lose focus");
}


void CWallDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	Print("Wall get focus");
}
