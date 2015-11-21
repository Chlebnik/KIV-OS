#pragma once
#include "stdafx.h"

using namespace std;

class AbstractProcess
{
protected:
	AbstractInput* input;
	AbstractOutput* output;
	AbstractOutput* errorOutput;
	Kernel* kernel;
	int pid;
	vector<string> parameters;

public:
	AbstractProcess(int pid, Kernel* kernel);
	virtual string GetHelpContent()
	{
		return "This program or command has no help content.";
	}
	virtual bool HasValidParameters() = 0;
	virtual int RunProcess() = 0;
	
	void Init(AbstractInput* input, AbstractOutput* output, AbstractOutput* errorOutput, vector<string> parameters);
	int Run();
	void WriteHelp();



};
