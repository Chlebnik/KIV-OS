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
	int parentPid;
	vector<string> parameters;
	void setParameters(string parameters);
	string path;


public:
	AbstractProcess(int pid, int parentPid, Kernel* kernel);
	virtual string GetHelpContent()
	{
		return "This program or command has no help content.";
	}
	virtual bool HasValidParameters() = 0;
	virtual int RunProcess() = 0;
	
	void Init(AbstractInput* input, AbstractOutput* output, AbstractOutput* errorOutput, string parameters);
	int Run();
	void WriteHelp();
	string GetPath();
	void SetPath(string path);
	int GetPid();
	int GetParentPid();
};
