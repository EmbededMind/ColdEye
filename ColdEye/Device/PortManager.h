#pragma once
#include "Device\Port.h"

class CPortManager 
{
private:
	CPortManager();
	~CPortManager();

public:
	CPort  mPorts[6];



	static CPortManager* GetInstance();

	void   LoadPortsParam();


	CPort*  GetPortById(int id);

    void    BindPortId(CPort* pPort, int id);

	CPort*  AllocPort();
};
