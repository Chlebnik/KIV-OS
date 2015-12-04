#pragma once
#include "stdafx.h"
using namespace std;

class AbstractIO
{
	
private:
	Kernel* kernel;

public:
	AbstractIO(Kernel* kernel);

	Kernel* GetKernel();

	virtual int Close() = 0;
};