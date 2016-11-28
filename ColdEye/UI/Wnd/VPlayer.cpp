#include "stdafx.h"
#include "VPlayer.h"

CVPlayer::CVPlayer()
{
}

CVPlayer::CVPlayer(CDuiString skinName)
{
	if (skinName)
		mSkinFileName = skinName;
}

CVPlayer::~CVPlayer()
{
}

LPCTSTR CVPlayer::GetWindowClassName() const
{
	return _T("VPlayer");
}

CDuiString CVPlayer::GetSkinFile()
{
	if(mSkinFileName)
		return mSkinFileName;
}

