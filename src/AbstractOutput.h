#pragma once

#include "stdafx.h"
using namespace std;

class AbstractOutput : public AbstractIO
{
	using AbstractIO::AbstractIO;

public:

	virtual int Write(string s) = 0;
	virtual int WriteLine(string s) = 0;
	
};
