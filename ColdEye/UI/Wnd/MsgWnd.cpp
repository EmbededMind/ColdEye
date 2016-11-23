#include "stdafx.h"
#include "MsgWnd.h"
#include "Control/RecordvoiceUI.h"
#include "ExHardDrive\ExHardDrive.h"


DUI_BEGIN_MESSAGE_MAP(CMsgWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CMsgWnd::CMsgWnd()
{
}


CMsgWnd::~CMsgWnd()
{
}


void CMsgWnd::SetTitle(LPCTSTR lpstrTitle)
{
	if (lstrlen(lpstrTitle) <= 0) return;
}

void CMsgWnd::SetMsg(LPCTSTR text1, LPCTSTR text2)
{
	if (SkinType == _T("mb_ok.xml") || SkinType==_T("mb_okcancel.xml")) {
		CControlUI* pText1 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text1")));
		CControlUI* pText2 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text2")));
		if (text1 != NULL) {
			pText1->SetText(text1);
			pText2->SetText(text2);
		}
		else {
			pText2->SetAttribute(_T("align"), _T("center"));
			pText2->SetText(text2);
		}
	}
	else if (SkinType == _T("mb_copyvideo_request.xml")) {
		CControlUI* pText2 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text2")));
		pText2->SetText(text1);
	}
	else if (SkinType == _T("mb_update_success.xml")) {
		CControlUI* pText1 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text1")));
		pText1->SetText(text1);
	}
	else if (SkinType == _T("mb_update.xml")) {
		CControlUI* pText1 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text1")));
		pText1->SetText(text1);
	}
	else if (SkinType == _T("mb_copyvideo.xml")) {
		CControlUI* pTitle = static_cast<CControlUI*>(m_pm.FindControl(_T("title")));
		pTitle->SetText(text1);
	}
	
}

void CMsgWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CMsgWnd::GetSkinFile()
{
	return SkinType;
}

LPCTSTR CMsgWnd::GetWindowClassName(void) const
{
	return _T("MsgWnd");
}

void CMsgWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	if (sName == _T("ok_btn")){
		Close(MSGID_OK);
	}
	else if (sName == _T("cancel_btn")) {
		Close(MSGID_CANCEL);
	}
	else if (sName == _T("record_btn")) {
		Close(MSGID_RECORD);
	}
}

LRESULT CMsgWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	CDuiString sName;
	switch (uMsg) {
	case WM_TIMER:
		lRes = OnTimer(uMsg,wParam,lParam,bHandled);
		break;
	//--------------------------------------------------------------------------
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (m_pm.GetFocus()) {
				sName = m_pm.GetFocus()->GetName();
				if (sName == _T("cancel_btn")) {
					if (m_pm.FindControl(_T("ok_btn"))) 
						m_pm.FindControl(_T("ok_btn"))->SetFocus();
				}
				else if (sName == _T("record_btn")) {
					if (m_pm.FindControl(_T("cancel_btn")))
						m_pm.FindControl(_T("cancel_btn"))->SetFocus();
				}
			}
			
			break;
		//--------------------------------------------------------------------------
		case VK_RIGHT:
			if (m_pm.GetFocus()) {
				sName = m_pm.GetFocus()->GetName();
				if (sName == _T("ok_btn")) {
					if (m_pm.FindControl(_T("cancel_btn")))
						m_pm.FindControl(_T("cancel_btn"))->SetFocus();
				}
				else if (sName == _T("cancel_btn")) {
					if (m_pm.FindControl(_T("record_btn")))
						m_pm.FindControl(_T("record_btn"))->SetFocus();
				}
			}
			break;
		//--------------------------------------------------------------------------
		case VK_BACK:
			if (!m_pm.GetFocus()) {
				Close(0);
			}
			break;
		//--------------------------------------------------------------------------
		case VK_RETURN: //test
			CRecordvoiceUI *pItem = static_cast<CRecordvoiceUI*>(m_pm.FindControl(_T("image")));
			if (pItem) {
				if (pItem->RecordTime != 60) {
					pItem->RecordTime = 60;
					KillTimer(m_pm.GetPaintWindow(), 1);
					Close(0);
				}
			}
			break;

		}
		break;

	//--------------------------------------------------------------------------
	case USER_MSG_RECORDVOICE: //开始录音
		{
			_time = 0;
			SetTimer(m_pm.GetPaintWindow(),1,500,NULL);
			CTextUI *pItem = static_cast<CTextUI*>(m_pm.FindControl(_T("Text2")));
			pItem->SetText(_T("松开          按键，结束录制。"));
		}

		break;
	//--------------------------------------------------------------------------
	case USER_MSG_STOP_RECORD://停止录音
		{
			CRecordvoiceUI *pItem = static_cast<CRecordvoiceUI*>(m_pm.FindControl(_T("image")));
			pItem->RecordTime = 60;
			KillTimer(m_pm.GetPaintWindow(), 1);
			Close(MSGID_OK);
		}
		break;
	//--------------------------------------------------------------------------
	case USER_MSG_COPY_INFO:
		{
			ProgressReflash();
			double sendSize = wParam;
			int num_progress = 100*(sendSize / (double)totalSize);
			CProgressUI* progress = (CProgressUI*)m_pm.FindControl(_T("copy_progress"));
			progress->SetMaxValue(num_progress);
		}
		break;

	case USER_MSG_COPY_STOP:
		if (wParam) {
			list<CRecordFileInfo*>::iterator iter;
			for (iter = pRecordInfo->begin(); iter != pRecordInfo->end(); iter++) {
				if ((CRecordFileInfo*)lParam == (*iter)) {
					totalSize -= (*iter)->dlSize;
					iter++;
					CExHardDrive::GetInstance()->CopyRecord((*iter), 0);
					return 0;
				}
			}
			Close(1);//复制结束
		}
		break;
	}


	bHandled = FALSE;
	return 0;
}

void CMsgWnd::ProgressReflash()
{
	int value;
	CProgressUI* progress = (CProgressUI*)m_pm.FindControl(_T("copy_progress"));
	value = progress->GetValue();
	if (value != progress->GetMaxValue()) {
		value++;
		progress->SetValue(value);
	}
	else {
		KillTimer(m_pm.GetPaintWindow(), 3);
	}
}

void CMsgWnd::RecordVoice()
{
	CRecordvoiceUI *pItem = static_cast<CRecordvoiceUI*>(m_pm.FindControl(_T("image")));
	CDuiString text;
	if (pItem->RecordTime != 0) {
		_time++;
		pItem->Blink = !pItem->Blink;
		if (_time == 2) {
			_time = 0;
			pItem->RecordTime = pItem->RecordTime--;
			text.Format(_T("%02d\""), pItem->RecordTime);
			pItem->SetText(text);
		}
		pItem->Invalidate();
	}
	else {
		pItem->RecordTime = 60;
		KillTimer(m_pm.GetPaintWindow(), 1);
		Close(MSGID_OK);
	}
}

void CMsgWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CMsgWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CMsgWnd::InitWindow()
{
	HWND MainDlg;
	pButton_ok = (CButtonUI*)m_pm.FindControl(_T("ok_btn"));
	pButton_cancel = (CButtonUI*)m_pm.FindControl(_T("cancel_btn"));
	pButton_record = (CButtonUI*)m_pm.FindControl(_T("record_btn"));
	if (SkinType == _T("mb_recordingvoice.xml")) {
		pMainDlg = (CColdEyeDlg*)AfxGetMainWnd();
		pMainDlg->mMessageBox = this;
		pMainDlg->SendMessage(USER_MSG_RECORDVOICE,NULL,NULL);
	}
	else if (SkinType == _T("mb_update.xml")) {
		UINT_PTR i;
		//i = SetTimer(m_pm.GetPaintWindow(),1, 200, NULL);
		i = SetTimer(m_pm.GetPaintWindow(),1, 200,NULL);
		printf("%d\n",i);
	}
	else if (SkinType == _T("mb_copyvideo.xml")) {
		totalSize = 0;
		list<CRecordFileInfo*>::iterator iter;
		for (iter = pRecordInfo->begin(); iter != pRecordInfo->end(); iter++)
			totalSize += (*iter)->dlSize;

		CExHardDrive::GetInstance()->CopyRecord(pRecordInfo->front(), 0);
	}
}

LRESULT CMsgWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam) {
		case 1: 
			RecordVoice();
			break;

		case 2:{
				CProgressUI *pItem = static_cast<CProgressUI*>(m_pm.FindControl(_T("update_progress")));
				int Value = pItem->GetValue();
				CDuiString text;
				if (Value < 100) {
					pItem->SetValue(Value++);
					text.Format(_T("%d%"), Value);
					pItem->SetText(text);
				}
				else {
					pItem->SetValue(0);
					text.Format(_T("%d%"), Value);
					pItem->SetText(text);
					KillTimer(m_pm.GetPaintWindow(), 2);
					Close(0);
				}
			}
			break;

		case 3:
			ProgressReflash();
			break;
	}
	
	bHandled = TRUE;
	return 0;
}
