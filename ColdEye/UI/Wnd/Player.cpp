#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}

CPlayer::CPlayer(CDuiString XML)
{
	sXml = XML;
}

CPlayer::~CPlayer()
{
}

LPCTSTR CPlayer::GetWindowClassName() const
{
	return _T("Player");
}

CDuiString CPlayer::GetSkinFile()
{
	return sXml;
}

LRESULT CPlayer::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (uMsg) {
	case USER_MSG_PLAY:
		long port;
		H264_PLAY_GetPort(&port);
		CString path;
		CRecordFileInfo *info = (CRecordFileInfo*)lParam;
		if (wParam == RECORD_NORMAl)
		{
			path = _T(NORMAL_RECORD_PATH);
		}
		else
		{
			path = _T(ALARM_RECORD_PATH);
		}
		CString tmp;
		CTime begintime = info->tBegin;
		tmp.Format(_T("%d\\%d%02d%02d%02d%02d%02d.h264"), info->nOwner, begintime.GetYear(), begintime.GetMonth(), begintime.GetDay(), 
			begintime.GetHour(), begintime.GetMinute(), begintime.GetSecond());
		path += tmp; Print("%S", path);
		USES_CONVERSION;
		const char * ptr = T2CA(path);
		H264_PLAY_OpenFile(port, ptr);
		H264_PLAY_Play(port,this->GetHWND());
		break;
	}

	return LRESULT();
}

void CPlayer::InitWindow()
{
	//pButton = (CButtonUI*)m_pm.FindControl(_T("player"));
}
