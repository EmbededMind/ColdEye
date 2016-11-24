// WallDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColdEye.h"
#include "WallDlg.h"
#include "afxdialogex.h"
#include "Com\Communication.h"

#include "Pattern\MsgSquare.h"


#include "Device\PortManager.h"

/**@brief ������Ϣ�ص�
 *
 */
bool __stdcall _cbDVRMessage(long loginId, char* pBuf, unsigned long bufLen, long dwUser)
{
	CWallDlg* pWall = (CWallDlg*)dwUser;

	ASSERT(pWall != NULL);

	return 0;
}


/**@brief ���߻ص�
 *
 */
void __stdcall _cbDisconnect(LONG loginId, char* szDVRIP, LONG DVRPort, DWORD userData)
{
	Print("---------------Disconnect-----------%s", szDVRIP);
	CWallDlg* pThis = (CWallDlg*)userData;
	ASSERT(pThis != nullptr);

	pThis->OnDisconnect(loginId, szDVRIP, DVRPort);
}


// CWallDlg �Ի���

IMPLEMENT_DYNAMIC(CWallDlg, CDialogEx)

CWallDlg::CWallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CWallDlg::~CWallDlg()
{
}

bool CWallDlg::Invest(CPort* pPort)
{
     ASSERT(pPort->m_Id > 0);
	 
	 int pos  = pPort->GetId() - 1;

	 ASSERT(mSurfaces[pos] == NULL);

	 mSurfaces[pos]  = new CSurface();
	 mSurfaces[pos]->Create(NULL, _T("Surface"), WS_CHILD|WS_VISIBLE|WS_BORDER|WS_THICKFRAME, CRect(0,0,0,0), this, pPort->GetId());
	 mSurfaces[pos]->ShowWindow(SW_SHOW);
	 mSurfaces[pos]->BindPort(pPort);
	 mSurfaces[pos]->ExecuteConfig();
	 DesignSurfaceLayout();
	 ExecuteSurfaceLayout();

	 return true;
}


/**@brief ��Ƶǽ�¼�һ������ͷ
 *
 */
//BOOL CWallDlg::Invest(CCamera* pCamera)
//{
////	pCamera->LoadLocalConfig();
////
////	pCamera->AttachPort(CPortManager::GetInstance()->GetPortById(1));
//
//	for (int i = 0; i < 6; i++) {
//		if (mSurfaces[i] == NULL) {
//			pCamera->m_Id = i + 1;
//
//			mSurfaces[i] = new CSurface();
//			mSurfaces[i]->Create(NULL, _T("Surface"), WS_CHILD|WS_VISIBLE|WS_BORDER|WS_THICKFRAME, CRect(0,0,0,0), this, pCamera->m_Id);
//			mSurfaces[i]->ShowWindow(SW_SHOW);
//
//
//			mSurfaces[i]->BindCamera(pCamera);
//
//			mSurfaces[i]->ExecuteLocalConfig();
//
//			DesignSurfaceLayout();
//			ExecuteSurfaceLayout();
//			return TRUE;
//		}
//	}
//
//	return FALSE;
//}



void CWallDlg::Delete(CSurface* pSurface)
{

}


/**@brief ��������ͷloginId�ҵ���Ӧ������ͷ��Surface
 *
 */
CSurface* CWallDlg::FindSurface(long loginId)
{
	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] != NULL) {
			//ASSERT(mSurfaces[i]->m_BindedCamera != NULL);


			//if (mSurfaces[i]->m_BindedCamera->GetLoginId() == loginId) {
			//	return mSurfaces[i];
			//}

			ASSERT(mSurfaces[i]->m_BindedPort != NULL);
			if (mSurfaces[i]->m_BindedPort->m_pCamera->GetLoginId() == loginId) {
				return mSurfaces[i];
			}
		}
	}

	return NULL;
}


/**@brief ������ƵǽӦ����ʾ���м���
 *
 */
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


/**@brief ����DesignSurfaceLayout����Ľ��ִ��Surface�Ĳ���
 *
 */
void CWallDlg::ExecuteSurfaceLayout()
{
	CRect rClient;
	GetClientRect(rClient);

	long nWidth = rClient.Width() / mCols;
	long nHeight = nWidth * 9 / 16;
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


/**@brief ��Ƶǽ�ĵ�����Ӧ
 *
 */
void CWallDlg::OnDisconnect(LONG loginId, char* szIp, LONG port)
{
	CSurface* pSurface = FindSurface(loginId);

	if (pSurface != NULL) {
		pSurface->OnDisconnect();
	}
}


void CWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWallDlg, CDialogEx)
	ON_MESSAGE(USER_MSG_LOGIN, &CWallDlg::OnUserMsgLogin)
	ON_MESSAGE(USER_MSG_LOGOFF, &CWallDlg::OnUserMsgLogoff)
END_MESSAGE_MAP()


// CWallDlg ��Ϣ�������


BOOL CWallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	H264_DVR_Init(_cbDisconnect, (DWORD)this);
	H264_DVR_SetDVRMessCallBack(_cbDVRMessage, (long)this);

	PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CWallDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			Print("Wall case key");
			for (int i = 0; i < 6; i++) {
				if (mSurfaces[i] != NULL) {
					mSurfaces[i]->SetFocus();
					return true;
				}
			}
			return true;

		default:
			if (GetKeyState(VK_CONTROL) && !(pMsg->lParam & 0x20000000)) {
				//CString text;
				//GetFocus()->GetWindowTextW(text);

				//TRACE("***%S\n", text);
				//CSurface* pSurface = (CSurface*)GetFocus();
				////CCamera* pDev = pSurface->m_BindedCamera;
				//if (pSurface != NULL) {
				//	Print("0x%p", pSurface);
				//}
				//CCamera* pDev  = pSurface->m_BindedPort->m_pCamera;

	/*			switch (pMsg->wParam)
				{
				case 'T':
					CCommunication::GetInstance()->AskTalk(pDev);
					return TRUE;

				case 'O':
					CCommunication::GetInstance()->YouTalk();
					return true;

				case 'S':
					CCommunication::GetInstance()->OverTalk();
					return true;

				default:
					break;*/
			/*	}*/
			}
			break;
		}
	}
	else if (pMsg->message == WM_CONTEXTMENU)
	{
		TRACE("CWallDlg case contextmenu\n");
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


/**@brief �豸��¼��Ӧ
 *
 */
afx_msg LRESULT CWallDlg::OnUserMsgLogin(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		Print("Login...");

		CPort* pPort  = CPortManager::GetInstance()->AllocPort();

		pPort->SetCamera( (CCamera*)lParam );

		Invest( pPort);        //��Ƶǽ����surface

		MSG msg;
		msg.message = USER_MSG_LOGIN;
		msg.lParam = (LPARAM)pPort;
		CMsgSquare::GetInstance()->Broadcast(msg);
	}
	else {
		AfxMessageBox(_T("��¼ʧ��"));
		delete ((CCamera*)lParam);
	}
	return 0;
}




/**@brief �豸ע����Ӧ
 *
 */
afx_msg LRESULT CWallDlg::OnUserMsgLogoff(WPARAM wParam, LPARAM lParam)
{
	CSurface* pSurface = (CSurface*)lParam;



	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] == pSurface) {
			//Delete(pSurface);

			MSG msg;
			msg.message = USER_MSG_LOGOFF;
			//msg.lParam = (LPARAM)pSurface->m_BindedCamera;
			msg.lParam = (LPARAM)pSurface->m_BindedPort->m_pCamera;
			CMsgSquare::GetInstance()->Broadcast(msg);


		/*	delete pSurface->m_BindedCamera;*/
		    delete pSurface->m_BindedPort->m_pCamera;
			delete mSurfaces[i];

			mSurfaces[i] = NULL;
			break;
		}
	}
	return 0;
}
