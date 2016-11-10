#pragma once

#include <map>
#include <list>
using namespace std;

//typedef std::pair<UINT, list<CWnd*> > TeamPair;


class CMsgSquare 
{
private:
	CMsgSquare() {}
	~CMsgSquare() {}

	std::map<UINT, list<HWND>* > mSquare;

public:
	static CMsgSquare* GetInstance();

	void        AddAudience(HWND, UINT);
	void        Broadcast(MSG& msg);
};

