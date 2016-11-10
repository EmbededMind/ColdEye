#pragma once

#include <list>
using namespace std;

class Subject;

class Observer
{
public:
	~Observer();

	virtual void Update(UINT opt, WPARAM wParam, LPARAM lParam) = 0;

protected:
	Observer();
};