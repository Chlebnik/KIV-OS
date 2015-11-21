#pragma once
#include "stdafx.h"
using namespace std;

class PipeOutput : public AbstractOutput
{
	using AbstractOutput::AbstractOutput;

private:
	int pipeIndex;
	bool closed;

public:
	PipeOutput(int pipeIndex, Kernel* kernel);
	int Close();
	int Write(string s);
	int WriteLine(string s);
	int GetPipeIndex();
};
