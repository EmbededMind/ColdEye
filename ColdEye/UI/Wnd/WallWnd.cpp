#include "stdafx.h"

#include "ColdEye.h"
#include "WallWnd.h"

#include "Pattern\MsgSquare.h"


/**@brief �豸������Ϣ�ص�����
*
*
*/
bool __stdcall cbDVRMessage(long lLoginID, char* pBuf, unsigned long swBufLen, long dwUser)
{
Print("Alarm message");
	CWallWnd* pWall = (CWallWnd*)dwUser;

	ASSERT(pWall != NULL);

	//CSurfaceUI* pSurface = pWall->FindSurface(lLoginID);
	//if (pSurface) {
	//	pSurface->OnAlarmTrigged();
	//}

	return 0;
}


void __stdcall cbDisConnect(LONG loginId, char* szDVRIP, LONG DVRPort, DWORD userData)
{
	Print("---------------- DisConnect ------------%s", szDVRIP);
	CWallWnd* pThis = (CWallWnd*)userData;

	ASSERT(pThis != nullptr);

	pThis->OnDisConnect(loginId, szDVRIP, DVRPort);
}




CWallWnd::CWallWnd()
{
}

CWallWnd::~CWallWnd()
{
}


LPCTSTR CWallWnd::GetWindowClassName() const
{
	return _T("WallWnd");
}


LRESULT CWallWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
		case WM_CREATE:{
				m_PaintManager.Init(m_hWnd);

				CDialogBuilder builder;
				CControlUI* pRoot = builder.Create(_T("wall.xml"), (UINT)0, NULL, &m_PaintManager);

				m_PaintManager.AttachDialog(pRoot);
				m_PaintManager.AddNotifier(this);


				m_pContainer = (CContainerUI*)m_PaintManager.FindControl(_T("vlSurface"));
				if (m_pContainer == NULL) {
					ASSERT(FALSE);
				}

				H264_DVR_Init(cbDisConnect, (DWORD)this);

				H264_DVR_SetDVRMessCallBack(cbDVRMessage, (long)this);

				CCamera* pCamera  = new CCamera();
				pCamera->m_Id;
				Invest(pCamera);

				DesignSurfaceLayout();
				ExecuteSurfaceLayout();

			}break;
		//----------------------------------------------------------------
		case USER_MSG_LOGIN:
			{
				if (wParam){
					TRACE("Login OK\n");
					Invest((CCamera*)lParam);
			    }

				MSG msg;
				msg.message = USER_MSG_TEST;
				msg.wParam = 2;
				msg.lParam = 3;
				CMsgSquare::GetInstance()->Broadcast(msg);
		    }break;
		//---------------------------------------------------------
		case USER_MSG_RELOGIN:{
				//CSurfaceUI* pSurface = FindSurface( ((CCamera*)lParam)->GetLoginId());
				//if (pSurface != NULL) {
				//	pSurface->OnReConnect();
				//}
			}			
			break;
		//---------------------------------------------------------
		case WM_KEYDOWN:
			TRACE("wall case key\n");
			switch (wParam)
			{
				case VK_UP:
					break;
				case VK_DOWN:
					break;
			}
			break;
	}

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)){
		return lRes;
	}


	return __super::HandleMessage(uMsg, wParam, lParam);
}



void CWallWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		TRACE("wall click\n");
	}
	else if (msg.sType == DUI_MSGTYPE_SETFOCUS) {
		//TRACE("wall  set focus\n");
	}
	else if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
		TRACE("wall kill focus\n");
	}
	else if (msg.sType == DUI_MSGTYPE_MENU) {
		TRACE("wall case -menu\n");
	}
	else {
		//TRACE("wall -> msg type:%S\n", msg.sType);
	}
}


/**@brief ��Ƶǽȥ����һ������ͷ
 *
 */
BOOL CWallWnd::Invest(CCamera* pCamera)
{
	pCamera->LoadLocalConfig();

	for (int i = 0; i < 6; i++){
		//if (mSurfaces[i] == NULL){
		if(mHolders[i] == NULL){
			pCamera->m_Id = i + 1;

			//mSurfaces[i] = new CSurfaceUI();

			//CWnd* pWnd = new CWnd();
			//
			//pWnd->Create(NULL, _T("Surface"), WS_CHILD | WS_VISIBLE, {0,0,0,0},CWnd::FromHandle(m_PaintManager.GetPaintWindow()),0);
			//mSurfaces[i]->Attach(pWnd->m_hWnd);

			//m_pContainer->Add(mSurfaces[i]);
			//mSurfaces[i]->SetBkColor(0xFFBDBDBD);
			//mSurfaces[i]->BindCamera(pCamera);
			////mSurfaces[i]->SetFocus();
			//mSurfaces[i]->ConnectRealPlay();

			//mSurfaces[i]->ExecuteLocalConfig();

			//DesignSurfaceLayout();
			//ExecuteSurfaceLayout();
			//return TRUE;


			mHolders[i] = new CSurfaceHolder();
			mHolders[i]->Create(IDD_HOLDER, CWnd::FromHandle(m_PaintManager.GetPaintWindow()));
			mHolders[i]->ShowWindow(SW_SHOW);

			mHolders[i]->BindCamera(pCamera);
			mHolders[i]->ConnectRealPlay();
			mHolders[i]->ExecuteLocalConfig();

			DesignSurfaceLayout();
			ExecuteSurfaceLayout();
			return TRUE;
		}
	}

	return FALSE;
}


/**@brief ����Surface�Ĳ���
 *
 */
void CWallWnd::DesignSurfaceLayout()
{
	int i = 0;
	int iSurfaceNumber  = 0;
	for (; i < 6; i++) {
		//if (mSurfaces[i] != NULL){
		//	iSurfaceNumber++;
		//}
		if (mHolders[i] != NULL) {
			iSurfaceNumber++;
		}
	}

	if (iSurfaceNumber == 0){
		mRows = 0;
		mCols = 0;
		return;
	}

	int qVal = 0;
	for (i = 1; ; i++){
		qVal = i*i;
		if (qVal >= iSurfaceNumber){
			mCols = i;
			break;
		}
	}

	mRows = iSurfaceNumber / mCols + (iSurfaceNumber%mCols)>0?1:0;
	TRACE("Rows:%d, Cols:%d\n", mRows, mCols);
}


/**@brief ִ��Surface����
 *
 */
void CWallWnd::ExecuteSurfaceLayout()
{
	CRect rClient;
	::GetClientRect(m_hWnd, rClient);
	
	long nWidth = rClient.Width() / mCols;
	long nHeight = nWidth * 8 / 16;
	long nOrgX = (rClient.Width() - nWidth*mCols) / 2;
	long nOrgY = (rClient.Height() - nHeight*mRows) / 2;

	int cnt = 0;
	for (int i = 0; i < 6; i++)
	{
		//if (mSurfaces[i] != NULL)
		//{
		//	long xPos = nOrgX + (cnt % mCols) * nWidth;
		//	long yPos = nOrgY + (cnt / mCols) * nHeight;

		//	mSurfaces[i]->SetFloat(true);
		//	mSurfaces[i]->SetFixedPos({ xPos, yPos, xPos + nWidth, yPos + nHeight });

		//	cnt++;
		//}


		if (mHolders[i] != NULL) {
			long xPos = nOrgX + (cnt % mCols) * nWidth;
			long yPos = nOrgY + (cnt /mCols)* nHeight;
			 
			mHolders[i]->SetWindowPos(NULL, xPos, yPos, nWidth, nHeight, 0);
			cnt++;
		}
	}

}




void CWallWnd::OnDisConnect(LONG loginId, char * szIp, LONG port)
{
//	CSurfaceUI* pSurface = FindSurface(loginId);
//
//	ASSERT(pSurface != NULL || "Find null ReConnnect surface" == 0);
//
//	pSurface->OnDisConnect();
}





//CSurfaceUI* CWallWnd::FindSurface(long logidId)
//{
//	for (int i = 0; i < 6; i++) {
//		if (mSurfaces[i] && mSurfaces[i]->m_BindedCamera  && mSurfaces[i]->m_BindedCamera->GetLoginId() == logidId) {
//			return mSurfaces[i];
//		}
//	}
//	
//	return NULL;
//}