#include "stdafx.h"

#include "Pattern\Subject.h"

Subject::Subject()
{}


Subject::~Subject()
{}


void Subject::Notify()
{
	Print("Subject gg");
}

void Subject::Attach(Observer* pObserver)
{
	this->mObserveres.push_back(pObserver);
}



void Subject::Detach(Observer* pObserver)
{
	list<Observer*>::iterator iter;

	iter = find(mObserveres.begin(), mObserveres.end(), pObserver);

	if (iter != mObserveres.end()) {
		mObserveres.erase(iter);
	}
}





