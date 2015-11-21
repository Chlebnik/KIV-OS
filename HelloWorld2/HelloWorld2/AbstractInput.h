#pragma once
#include "stdafx.h"
using namespace std;

class AbstractInput : public AbstractIO
{
	using AbstractIO::AbstractIO;

public:
	
	virtual bool HasNext()	= 0;
	virtual string Read() = 0;
	virtual string ReadLine(bool& success) = 0;
};
