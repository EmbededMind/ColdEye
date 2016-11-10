#include "stdafx.h"
#include "ColdEye.h"

#include "Wnd\MainWnd.h"
#include "Control\UISurface.h"

#include "Device\Camera.h"

#include "Pattern\MsgSquare.h"



CMainWnd::CMainWnd()
{
}


CMainWnd::~CMainWnd()
{
}



LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
		case WM_CREATE:
		{
			m_PaintManager.Init(m_hWnd);

			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("main.xml"), (UINT)0, NULL, &m_PaintManager);
			
			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);

			CHorizontalLayoutUI* pVerLayout = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("hlTitle"));

			CRect rClient;
			::GetClientRect(m_hWnd, rClient);


			m_pWall = new CWallWnd();
			m_pWall->Create(m_hWnd, _T("Wall"), UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE);
			::MoveWindow(m_pWall->GetHWND(), 0, pVerLayout->GetFixedHeight(), rClient.Width(), rClient.Height(), TRUE);
			m_pWall->ShowWindow();

			((CColdEyeApp*)AfxGetApp())->SetWallWnd(m_pWall);

			PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_SCAN_DEV, 0, 0);

			CMsgSquare::GetInstance()->AddAudience(m_hWnd, USER_MSG_TEST);
		}
		break;


		case USER_MSG_SCAN_DEV:
			TRACE("Case scan dev msg\n");
			{

				for (int i = 0; i < wParam; i++)
				{
					CCamera* pCamera = new CCamera();
					pCamera->SetCommonNetConfig( &((SDK_CONFIG_NET_COMMON_V2*)lParam)[i]);
					PostThreadMessage( ((CColdEyeApp*)AfxGetApp())->GetLoginThreadPID(), USER_MSG_LOGIN, 0, (LPARAM)pCamera);
				}
			}
			break;

		case USER_MSG_TEST:
			Print("Rec test msg:%d,%d", wParam, lParam);
			break;
	}


	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}


void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnSysDateTime"))
		{
			::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
		}
	}
}


CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, _T("Surface")) == 0)
	{
		TRACE("Find SurfaceUI\n");
		CSurfaceUI* pUI = new CSurfaceUI();
		HWND hWnd = CreateWindow(_T("CWnd"), _T("win32"), WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);
		return pUI;
	}

	return NULL;
}


