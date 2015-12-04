#pragma once
#include "stdafx.h"
using namespace std;

class StandardOutput : public AbstractOutput
{
	using AbstractOutput::AbstractOutput;

public:
	int Write(string s);
	int WriteLine(string s);
	int Close();
};