// WallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "WallDlg.h"
#include "afxdialogex.h"
#include "Com\Communication.h"

#include "Pattern\MsgSquare.h"


#include "Device\PortManager.h"

/**@brief 报警消息回调
 *
 */
bool __stdcall _cbDVRMessage(long loginId, char* pBuf, unsigned long bufLen, long dwUser)
{
	CWallDlg* pWall = (CWallDlg*)dwUser;

	ASSERT(pWall != NULL);

	CSurface* pSurface  = pWall->FindSurface(loginId);

	if (pSurface) {
		pSurface->OnAlarmTrigged();
	}

	return 0;
}


/**@brief 掉线回调
 *
 */
void __stdcall _cbDisconnect(LONG loginId, char* szDVRIP, LONG DVRPort, DWORD userData)
{
	Print("---------------Disconnect-----------%s", szDVRIP);
	CWallDlg* pThis = (CWallDlg*)userData;
	ASSERT(pThis != nullptr);

	PostMessage(pThis->m_hWnd, USER_MSG_DISCONNECT, 0,  loginId);
	//pThis->OnDisconnect(loginId, szDVRIP, DVRPort);
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

bool CWallDlg::Ingest(CPort* pPort)
{
     ASSERT(pPort->m_Id > 0);
	 
	 int pos  = pPort->GetId() - 1;

	 ASSERT(mSurfaces[pos] == NULL);

	 mSurfaces[pos]  = new CSurface();
	 mSurfaces[pos]->Create(NULL, _T("Surface"), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0,0,0,0), this, pPort->GetId());
	 mSurfaces[pos]->ShowWindow(SW_SHOW);


	 mSurfaces[pos]->BindPort(pPort);
	 mSurfaces[pos]->ExecuteConfig();

	 DesignSurfaceLayout();
	 ExecuteSurfaceLayout();

	 return true;
}



void CWallDlg::Delete(CSurface* pSurface)
{

}


/**@brief 根据设想头loginId找到对应此摄像头的Surface
 *
 */
CSurface* CWallDlg::FindSurface(long loginId)
{
	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] != NULL) {

			ASSERT(mSurfaces[i]->m_BindedPort != NULL);
			if (mSurfaces[i]->m_BindedPort->m_pCamera->GetLoginId() == loginId) {
				return mSurfaces[i];
			}
		}
	}

	return NULL;
}


CSurface* CWallDlg::FindSurface(CPort* pPort)
{
	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] != NULL) {
			ASSERT(mSurfaces[i]->m_BindedPort != NULL);
			if (mSurfaces[i]->m_BindedPort == pPort) {
				return mSurfaces[i];
			}
		}
	}
	return NULL;
}


/**@brief 计算视频墙应该显示几行几列
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
	i  = 0;
	while (i <= iSurfaceNumber) {
		qVal  = i*i;
		if (qVal >= iSurfaceNumber) {
			mCols  = i;
			break;
		}
		i++;
	}

	mRows = iSurfaceNumber / mCols + ((iSurfaceNumber%mCols) > 0 ? 1 : 0);
	Print("Rows:%d, Cols:%d", mRows, mCols);
}


/**@brief 根据DesignSurfaceLayout计算的结果执行Surface的布局
 *
 */
void CWallDlg::ExecuteSurfaceLayout()
{
	CRect rClient;
	GetClientRect(&rClient);


	float hMargin  = rClient.Width() /20.0;
	float vMargin  = rClient.Height() /10.0;

	float grap     = rClient.Height() /40.0;

	float hLimit   = (rClient.Width() - 2*(hMargin + grap*mCols)) /4 / mCols;
	float vLimit   = (rClient.Height() - 2*(vMargin + grap*mRows)) /3 / mRows;
	float limit  = hLimit > vLimit ? vLimit : hLimit;

	int yOrg  = (rClient.Height() - (3*limit+2*grap)*mRows) / 2;
	int xOrg  = (rClient.Width() - (4*limit+2*grap)*mCols) / 2;


	int  width = 4 * limit;
	int  height = 3 * limit;

    CRect r;

	int cnt = 0;
	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] != NULL) {

			long xPos   = xOrg + (cnt % mCols) *(width+grap*2);
			long yPos   = yOrg + (cnt / mCols) *(height+grap*2);
			
			r.left  = xPos + grap;
			r.right = r.left + width;
			r.top   = yPos + grap;
			r.bottom= r.top + height;
			//ClientToScreen(r);
			/*mSurfaces[i]->SetWindowPos(NULL, xPos+grap, yPos+grap, width, height, 0);*/
			mSurfaces[i]->SetPos(r);
			cnt++;
		}
	}
}


void CWallDlg::DeleteSurface(CSurface* pSurface)
{
	pSurface->Delete();

	for (int i = 0; i < 6; i++) {
		if (mSurfaces[i] == pSurface) {
			mSurfaces[i]  = NULL;
			delete pSurface;

			DesignSurfaceLayout();
			ExecuteSurfaceLayout();

			return;
		}
	}
}



void CWallDlg::TestIngestOne()
{
	static int Id  = 0;

	if(Id >= 6)
		return;

	Id++;
	CPort * pNewPort  = new CPort();
	pNewPort->SetId(Id);
	Ingest(pNewPort);
}




void CWallDlg::TestSplitOne()
{

}



void CWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWallDlg, CDialogEx)
	ON_MESSAGE(USER_MSG_LOGIN, &CWallDlg::OnUserMsgLogin)
	ON_MESSAGE(USER_MSG_LOGOFF, &CWallDlg::OnUserMsgLogoff)
	ON_MESSAGE(USER_MSG_CAMERA_CONFIG_CHANGE, &CWallDlg::OnUserMsgCameraConfigChange)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(USER_MSG_DISCONNECT, &CWallDlg::OnUserMsgDisconnect)
	ON_MESSAGE(USER_MSG_RELOGIN, &CWallDlg::OnUserMsgRelogin)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWallDlg 消息处理程序


BOOL CWallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	H264_DVR_Init(_cbDisconnect, (DWORD)this);
	H264_DVR_SetDVRMessCallBack(_cbDVRMessage, (long)this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CWallDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case VK_UP:
		TestIngestOne();
		break;
		//-----------------------
		case VK_DOWN:
		break;
		//-----------------------
		case VK_RETURN:
		return true;
		break;
		//-----------------------
		default:
			break;
		}
	}
	else if (pMsg->message == WM_CONTEXTMENU)
	{
		TRACE("CWallDlg case contextmenu\n");
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}







/**@brief 设备注销响应
 *
 */
afx_msg LRESULT CWallDlg::OnUserMsgLogoff(WPARAM wParam, LPARAM lParam)
{
	CPort* pPort  = (CPort*)lParam;
	if (wParam == 2) {
		CSurface* pSurface  = FindSurface(pPort);

		if (pSurface != NULL) {
			DeleteSurface(pSurface);
		}

		MSG msg;
		msg.message  = USER_MSG_LOGOFF;
		msg.lParam   = (LPARAM)pPort;
		CMsgSquare::GetInstance()->Broadcast(msg);

		return 0;
	}


	//CSurface* pSurface = (CSurface*)lParam;



	//for (int i = 0; i < 6; i++) {
	//	if (mSurfaces[i] == pSurface) {
	//		//Delete(pSurface);

	//		MSG msg;
	//		msg.message = USER_MSG_LOGOFF;
	//		//msg.lParam = (LPARAM)pSurface->m_BindedCamera;
	//		msg.lParam = (LPARAM)pSurface->m_BindedPort->m_pCamera;
	//		CMsgSquare::GetInstance()->Broadcast(msg);


	//	/*	delete pSurface->m_BindedCamera;*/
	//	    delete pSurface->m_BindedPort->m_pCamera;
	//		delete mSurfaces[i];

	//		mSurfaces[i] = NULL;
	//		break;
	//	}
	//}
	return 0;
}


afx_msg LRESULT CWallDlg::OnUserMsgCameraConfigChange(WPARAM wParam, LPARAM lParam)
{
	CPort* pPort  = (CPort*)wParam;
	DeviceConfig* pConfig  = (DeviceConfig*)lParam;

	ASSERT(pPort != NULL);
	ASSERT(pPort->m_pCamera != NULL);

	if (pConfig->Volumn != pPort->m_DevConfig.Volumn) {
		::SendMessage(AfxGetMainWnd()->m_hWnd, USER_MSG_CAMERA_CONFIG_CHANGE, wParam, lParam);
	}
	
	CSurface* pSurface  = FindSurface(pPort);
	if (pSurface != NULL) {
		pPort->m_DevConfig.NameId      = pConfig->NameId;
		pPort->m_DevConfig.IsCameraOn  = pConfig->IsCameraOn;
		pPort->m_DevConfig.NameId  = pConfig->NameId;
		pPort->m_DevConfig.Volumn  = pConfig->Volumn;
		pPort->m_DevConfig.IsRecordEnabled  = pConfig->IsRecordEnabled;
		pPort->m_DevConfig.IsAutoWatchEnabled  = pConfig->IsAutoWatchEnabled;

		pPort->StoreDeviceConfig();

		pSurface->ExecuteConfig();
	}
	return 0;
}


void CWallDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	Print("----Reconnect----");

	if (mReconnectPort.size() > 0) {
		CPort * pPort   = mReconnectPort.front();
		PostThreadMessage(((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_RELOGIN, (WPARAM)this->m_hWnd, (LPARAM)pPort);
		mReconnectPort.pop_front();
	}

	if (mReconnectPort.size() == 0) {
		KillTimer(TIMER_ID_RECONNECT);
	}

	CDialogEx::OnTimer(nIDEvent);
}




/**@brief 视频墙的掉线响应
*
*/
void CWallDlg::OnDisconnect(LONG loginId, char* szIp, LONG port)
{
	CSurface* pSurface = FindSurface(loginId);

	if (pSurface != NULL) {
		CPort* pPort = pSurface->m_BindedPort;
		if (pPort) {
			// 拔出又掉线，可以删除矣。
			if (pPort->m_State == OFFLINE) {
				Print("Disconnect and offline");

				CUtil::RemoveDev(pPort->m_pCamera);

				DeleteSurface(pSurface);

				pPort->m_pCamera = NULL;

				MSG msg;
				msg.message = USER_MSG_LOGOFF;
				msg.lParam = (LPARAM)pPort;
				CMsgSquare::GetInstance()->Broadcast(msg);
			}
			// 掉线未拔出，坐等重连
			else {
				Print("Disconnect but online");

				pSurface->OnDisconnect();


				if (mReconnectPort.size() == 0) {
					SetTimer(TIMER_ID_RECONNECT, 10 * 1000, NULL);
				}
				mReconnectPort.push_back(pPort);
			}
		}
		else {
			DeleteSurface(pSurface);
			Print("Disconnect surface has no port,what fuck!");
		}
	}
}


afx_msg LRESULT CWallDlg::OnUserMsgDisconnect(WPARAM wParam, LPARAM lParam)
{
	this->OnDisconnect(lParam,  NULL, 0);
	return 0;
}



/**@brief 设备登录响应
*
*/
afx_msg LRESULT CWallDlg::OnUserMsgLogin(WPARAM wParam, LPARAM lParam)
{
	CPort* pPort = (CPort*)lParam;

	
	CUtil::LoadMap(pPort->m_pCamera);

	if (wParam) {
		Print("Login succeed");

		pPort->m_State = ONLINE;

		Ingest(pPort);        //视频墙增加surface

		MSG msg;
		msg.message = USER_MSG_LOGIN;
		msg.lParam = lParam;
		CMsgSquare::GetInstance()->Broadcast(msg);
	}
	else {
		Print("----Login failed----");
		/*delete ((CCamera*)lParam);*/

		if (mReconnectPort.size() == 0) {
			SetTimer(TIMER_ID_RECONNECT, 10 * 1000, NULL);
		}
		mReconnectPort.push_back(pPort);
		
	}
	return 0;
}


afx_msg LRESULT CWallDlg::OnUserMsgRelogin(WPARAM wParam, LPARAM lParam)
{
	CPort* pPort  = (CPort*) lParam;
	
	if (! wParam) {
		Print("Reconnect failed");
		// 掉线的情况下 是拔出则删除
		if (pPort->m_State == OFFLINE) {
			CSurface* pSurface = FindSurface(pPort);
			if (pSurface) {
				
				CUtil::RemoveDev(pPort->m_pCamera);

				DeleteSurface(pSurface);
				pPort->m_pCamera = NULL;

				MSG msg;
				msg.message = USER_MSG_LOGOFF;
				msg.lParam = (LPARAM)pPort;
				CMsgSquare::GetInstance()->Broadcast(msg);
			}
		}
		else {
			if (FindSurface(pPort) == NULL) {
				Ingest(pPort);
			}

			// 10s 重连。
			if (mReconnectPort.size() == 0) {
				SetTimer(TIMER_ID_RECONNECT, 10 * 1000, NULL);
			}

			mReconnectPort.push_back(pPort);
		}
	}
	else {
		Print("Reconnect succeed");
		CSurface* pSurface  = FindSurface(pPort);
		if (pSurface) {
			pSurface->OnReconnect();
		}
	}

	return 0;
}

BOOL CWallDlg::OnEraseBkgnd(CDC * pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, RGB(64,84,115));
	//return CFrameWnd::OnEraseBkgnd(pDC);   
	return TRUE;
}

void CWallDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	//dc.Rectangle(mSurfaceArea);
}
