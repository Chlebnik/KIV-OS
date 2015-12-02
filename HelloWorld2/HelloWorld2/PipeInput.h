#pragma once
#include "stdafx.h"
using namespace std;

class PipeInput : public AbstractInput
{
	using AbstractInput::AbstractInput;

private:
	int pipeIndex;
	bool closed;

public:
	PipeInput(int pipeIndex, shared_ptr<Kernel> kernel);
	int Close();
	bool HasNext();
	string Read();
	string ReadLine(bool& success);
};
