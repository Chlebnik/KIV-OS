#pragma once
#include "stdafx.h"
using namespace std;

class StandardInput : public AbstractInput
{
	using AbstractInput::AbstractInput;

private:
	bool closed;

public:
	int Close();
	bool HasNext();
	string Read();
	string ReadLine(bool& success);
};