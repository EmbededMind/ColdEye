#include "stdafx.h"
#include "MsgSquare.h"




CMsgSquare * CMsgSquare::GetInstance()
{
	static CMsgSquare mInstance;
	return &mInstance;
}



void   CMsgSquare::AddAudience(HWND audience, UINT interest)
{
	map<UINT, list<HWND>* >::iterator iter = mSquare.find(interest);

	if (iter != mSquare.end()) {
		//iter->second->push_back(audience);
		iter->second->push_back(audience);
	}
	else {
		list<HWND>* pNewTeam  = new list<HWND>;
		pNewTeam->push_back(audience);
		
		//mSquare.insert(interest, pNewTeam);
		mSquare.insert(make_pair(interest, pNewTeam));
	}
}



void CMsgSquare::Broadcast(MSG& msg)
{
	map<UINT, list<HWND>* >::iterator iter = mSquare.find(msg.message);
	if (iter != mSquare.end()) {
		list<HWND>::iterator it = iter->second->begin();

		for (; it != iter->second->end(); it++) {
			::SendMessage( *it, msg.message, msg.wParam, msg.lParam );
		}
	}
}



void CMsgSquare::RemoveAudience(HWND hWnd)
{
	map<UINT, list<HWND>* >::iterator iter  = mSquare.begin();

	for (iter; iter != mSquare.end(); iter++) {
		iter->second->remove(hWnd);
	}
}
