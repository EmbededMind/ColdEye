#include "stdafx.h"
#include "MsgWnd.h"
#include "Control/RecordvoiceUI.h"
#include "ExHardDrive\ExHardDrive.h"
#include "Com\RecordAlarmSound.h"
#include "Com\MCI.h"
#include "Device\PortManager.h"

DUI_BEGIN_MESSAGE_MAP(CMsgWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()


CMsgWnd::CMsgWnd()
{
	pRecordInfo = NULL;
}

CMsgWnd::~CMsgWnd()
{
	if(!IsChildren)
		pMainDlg->mMessageBox = NULL;
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
	else if (SkinType == _T("mb_playvoice.xml")) {
		CControlUI* pText1 = static_cast<CControlUI*>(m_pm.FindControl(_T("contain")));
		if(_tcscmp(text1,_T("录制报警音")) == 0)	
			pText1->SetText(_T("是否确认创建录制语音？"));
		else
			pText1->SetText(_T("是否确认替换原录制语音？"));
	}
}



LRESULT CMsgWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsChildren) {
		if (wParam) {
			::PostMessage(((CColdEyeDlg*)AfxGetMainWnd())->mMessageBox->GetHWND(), USER_MSG_CANCEL_COPY, NULL, NULL);
		}

	}
	__super::OnClose( uMsg,  wParam,  lParam,  bHandled);
	bHandled = FALSE;
	return 0;
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
	else if (sName == _T("cancel_copy")) {
		pChildWnd =	ShowMessageBox(m_hWnd, _T("mb_okcancel.xml"), NULL, _T("确定停止复制视频？"));
	}
}

LRESULT CMsgWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	CDuiString sName;
	CTextUI *pItem;
	switch (uMsg) {
		
	case DOF_EXECUTABLE:{
			CControlUI* pText1 = static_cast<CControlUI*>(m_pm.FindControl(_T("Text1")));
			if (!pText1) break;
			if(pText1->GetText()==_T("关闭储存摄像机视频后，你将无法回")){
				pButton_cancel->SetFocus();
			}
			if (pText1->GetText() == _T("关闭摄像机自动看船后，该摄像头将")) {
				pButton_cancel->SetFocus();
			}
		}

		break;
	//--------------------------------------------------------------------------
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
				if(SkinType!=_T("mb_recordingvoice.xml"))
					Close(0);
			}
			break;
		//--------------------------------------------------------------------------
		case VK_RETURN:
			{
				CRecordvoiceUI *pItem = static_cast<CRecordvoiceUI*>(m_pm.FindControl(_T("image")));
				if (pItem) {
					if (pItem->RecordTime != 60) {
						pItem->RecordTime = 60;
						KillTimer(m_hWnd, TIME_RECORD_VOICE);
						Close(0);
					}
				}
			}
			break;
		default:
			if (SkinType == _T("mb_recordingvoice.xml")) {
				if (GetKeyState(VK_CONTROL) && !(lParam & 0x20000000)) {
					switch (wParam) {
					//开始录制报警音
					case 'T': 
						{
							CTextUI *pItem = static_cast<CTextUI*>(m_pm.FindControl(_T("Text2")));
							_time = 0;
							SetTimer(m_hWnd, TIME_RECORD_VOICE, 500, NULL);
							pItem->SetText(_T("松开          按键，结束录制。"));
							CCamera* pCamera = NULL;
							for (int i = 0; i < 6; i++)
							{
								if (CPortManager::GetInstance()->mPorts[i].GetCamera())
								{
									pCamera = CPortManager::GetInstance()->mPorts[i].GetCamera();
									break;
								}
							}
							if (pCamera)
							{
								CRecordAlarmSound::GetInstance()->Record(pCamera);
								CMCI::GetInstance()->StartRecord();
							}
						}
						break;

					case 'O':
						{
							CRecordvoiceUI *pItem = static_cast<CRecordvoiceUI*>(m_pm.FindControl(_T("image")));
							pItem->RecordTime = 60;
							KillTimer(m_hWnd, TIME_RECORD_VOICE);
							CRecordAlarmSound::GetInstance()->StopTalk();
							CMCI::GetInstance()->StopRecord();
							Close(MSGID_OK);
						}
						break;
					}
				}
			}

			break;

		}
		break;
	//--------------------------------------------------------------------------
	case USER_MSG_COPY_INFO:
		{
			CDuiString text;
			int Size = sendedSize + wParam;
			ProgressReflash(Size);
		}
		break;

	case USER_MSG_COPY_STOP:
		{
		if (!pRecordInfo) break;
			list<CRecordFileInfo*>::iterator iter;
			for (iter = pRecordInfo->begin(); iter != pRecordInfo->end(); iter++) {
				if ((CRecordFileInfo*)lParam == (*iter)) {
					sendedSize += (*iter)->dlSize;
					ProgressReflash(sendedSize);
					iter++;
					if (iter == pRecordInfo->end()) {
						CProgressUI* progress = (CProgressUI*)m_pm.FindControl(_T("copy_progress"));
						progress->SetValue(totalSize);
						if (pChildWnd)
							pChildWnd->Close();
						Close(1);//复制结束
						return 0;
					}
					CExHardDrive::GetInstance()->CopyRecord((*iter), videoType);
					return 0;
				}
			}
			if (pChildWnd)
				pChildWnd->Close();
			Close(1);//复制结束
		}
		break;

	case USER_MSG_EXHARDDRIVE_OUT:
		if (pChildWnd)
			pChildWnd->Close();
		CExHardDrive::GetInstance()->CancelCopy();
		Close(MSGID_EXHARDDRIVE_OUT);
		break;

	case USER_MSG_CANCEL_COPY:
		CExHardDrive::GetInstance()->CancelCopy();
		Close(0);
		break;
	}


	bHandled = FALSE;
	return 0;
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
	// 时间到
	else {
		pItem->RecordTime = 60;
		CRecordAlarmSound::GetInstance()->StopTalk();
		CMCI::GetInstance()->StopRecord();
		KillTimer(m_hWnd, TIME_RECORD_VOICE);
		Close(MSGID_OK);
	}
}



void CMsgWnd::AlarmVoicePlay()
{
	static_cast<CSliderUI*>(m_pm.FindControl(_T("voice_slider")));
	int TotalTime;
	int PlayTime;
	CMCI *pCmic = CMCI::GetInstance();
	TotalTime = pCmic->GetTotaltime();
	PlayTime = pCmic->GetPlayTime();
Print("alarm voice PlayTime:%d",PlayTime);
	static_cast<CSliderUI*>(m_pm.FindControl(_T("voice_slider")))->SetValue(PlayTime);
	if (PlayTime == TotalTime)
		KillTimer(m_hWnd, TIME_PLAY_VOICE);
}

void CMsgWnd::ProgressReflash(int Size)
{
	CDuiString text;
	int num_progress = 100 * ((double)Size / (double)totalSize);
	CProgressUI* progress = (CProgressUI*)m_pm.FindControl(_T("copy_progress"));
	progress->SetValue(num_progress);
	text.Format(_T("%d%%"), num_progress);
	progress->SetText(text);
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
	pMainDlg = (CColdEyeDlg*)AfxGetMainWnd();
	if (!IsChildren)
		pMainDlg->mMessageBox = this;
	m_pm.SetDPI(((CColdEyeDlg*)AfxGetMainWnd())->mMenu.GetDpi());

	pButton_ok = (CButtonUI*)m_pm.FindControl(_T("ok_btn"));
	pButton_cancel = (CButtonUI*)m_pm.FindControl(_T("cancel_btn"));
	pButton_record = (CButtonUI*)m_pm.FindControl(_T("record_btn"));
	if (SkinType == _T("mb_recordingvoice.xml")) {
	}
	else if (SkinType == _T("mb_update.xml")) {
		UINT_PTR i;
		i = SetTimer(m_pm.GetPaintWindow(),1, 200,NULL);
	}
	else if (SkinType == _T("mb_copyvideo.xml")) {
		totalSize = 0;
		sendedSize = 0;
		list<CRecordFileInfo*>::iterator iter;
		for (iter = pRecordInfo->begin(); iter != pRecordInfo->end(); iter++)
			totalSize += (*iter)->dlSize;

		Print("Biu----copy video");
		CExHardDrive::GetInstance()->CopyRecord(pRecordInfo->front(), videoType);
	}
	else if (SkinType == _T("mb_playvoice.xml")) {
		int TotalTime;
		CMCI *pCmic = CMCI::GetInstance();
		pCmic->Play(RECORD_VOICE_TMP);
		TotalTime = pCmic->GetTotaltime();
		static_cast<CSliderUI*>(m_pm.FindControl(_T("voice_slider")))->SetMaxValue(TotalTime);
		SetTimer(m_hWnd, TIME_PLAY_VOICE , 1000, NULL);
	}

}

LRESULT CMsgWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam) {
		case TIME_RECORD_VOICE: 
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
					KillTimer(m_hWnd, 2);
					Close(0);
				}
			}
			break;

		case TIME_PLAY_VOICE:
			AlarmVoicePlay();
			break;
	}
	
	bHandled = TRUE;
	return 0;
}
