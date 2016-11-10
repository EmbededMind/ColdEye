#pragma once


#include <list>
#include "Pattern\Observer.h"

using namespace std;

class Observer;

class Subject {
public:
	~Subject();

	virtual void Notify();;
	virtual void Attach(Observer*);
	virtual void Detach(Observer*);


protected:
	Subject();

	list<Observer*> mObserveres;
};
