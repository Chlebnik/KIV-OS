#pragma once
#include "stdafx.h"
using namespace std;

class AbstractIO
{
	
private:
	shared_ptr<Kernel> kernel;

public:
	AbstractIO(shared_ptr<Kernel> kernel);

	shared_ptr<Kernel> GetKernel();

	virtual int Close() = 0;
};