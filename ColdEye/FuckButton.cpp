// FuckButton.cpp : ʵ���ļ�
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



// CFuckButton ��Ϣ�������




BOOL CFuckButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN) {
		::SendMessage(GetParent()->m_hWnd,  USER_MSG_NOFITY_KEYDOWN,  pMsg->wParam,  (LPARAM)this);
		return true;
	}

	else if (pMsg->message == WM_CONTEXTMENU) {
		return true;
	}

	return CButton::PreTranslateMessage(pMsg);
}



