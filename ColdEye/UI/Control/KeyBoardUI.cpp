#include "stdafx.h"
#include "conio.h" 

#include "Control\KeyBoardUI.h"
#include "Wnd\MsgWnd.h"

IMPLEMENT_DUICONTROL(CKeyBoardUI)
CKeyBoardUI::CKeyBoardUI()
{
}


CKeyBoardUI::~CKeyBoardUI()
{
}

void CKeyBoardUI::DoEvent(TEventUI &event)
{
	int userdata,line,row;
	CVerticalLayoutUI *pLayout;	//���岼��
	CHorizontalLayoutUI *pLineLayout; //�в���
	if (event.Type == UIEVENT_KEYDOWN)
	{
		userdata = StrToInt(GetUserData());
		line = userdata / 10;
		row = userdata % 10;
		CVerticalLayoutUI *pLayout = static_cast<CVerticalLayoutUI*>(m_pManager->FindControl(_T("keyboard")));
		CButtonUI* pEdit = (CButtonUI*)m_pManager->FindControl(_T("edit_shipname"));
		switch (event.wParam)
		{
		case VK_UP:
			if (line > 0)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line-1);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row));
			}
			break;

		case VK_DOWN:
			if (line < 3)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line+1);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row));
			}
			break;

		case VK_LEFT:
			if (row > 0)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row-1));
			}
			break;

		case VK_RIGHT:
			if (row < 9)
			{
				pLineLayout = (CHorizontalLayoutUI*)pLayout->GetItemAt(line);
				m_pManager->SetFocus(pLineLayout->GetItemAt(row + 1));
			}
			break;

		case VK_RETURN: {
			CDuiString oldText, newText;
			oldText = pEdit->GetText();
			newText = GetText();
			if (newText.GetLength() > 0) {
				if ((pEdit->GetText().GetLength()) < 32) {
					oldText.Append(newText);
					pEdit->SetText(oldText);
				}
				else {
					CMsgWnd::MessageBox(m_pManager->GetPaintWindow(), _T("mb_shipname_input.xml"), NULL, NULL);
				}
			}
			else {//ɾ������
				if (row == 9) {//ȫ��ɾ��
					pEdit->SetText(_T(""));
				}
				else {//ɾ��һλ
					if (oldText.GetLength() > 0) {
						oldText.Assign(oldText, oldText.GetLength() - 1);
						pEdit->SetText(oldText);
					}

				}
			}
		}
		break;

		case VK_BACK:
			GetParent()->GetParent()->SetVisible(false);
			m_pManager->FindControl(_T("prompt"))->SetVisible(false);
			pEdit->SetFocus();
			break;

		}
	}
	CButtonUI::DoEvent(event);
}
