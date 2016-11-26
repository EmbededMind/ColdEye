// FuckButton.cpp : 实现文件
//

#include "stdafx.h"
#include "ColdEye.h"
#include "FuckButton.h"


// CFuckButton

IMPLEMENT_DYNAMIC(CFuckButton, CButton)

CFuckButton::CFuckButton()
{

}

CFuckButton::~CFuckButton()
{
}


BEGIN_MESSAGE_MAP(CFuckButton, CButton)
END_MESSAGE_MAP()



// CFuckButton 消息处理程序




BOOL CFuckButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) {
		::SendMessage(GetParent()->m_hWnd,  USER_MSG_NOFITY_KEYDOWN,  pMsg->wParam,  (LPARAM)this);
		return true;
	}

	else if (pMsg->message == WM_CONTEXTMENU) {
		return true;
	}

	return CButton::PreTranslateMessage(pMsg);
}



