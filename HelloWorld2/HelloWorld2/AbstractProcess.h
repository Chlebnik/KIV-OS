#pragma once
#include "stdafx.h"

using namespace std;

class AbstractInput;
class AbstractOutput;
class Kernel;

class AbstractProcess
{
private:
	thread* t;
	int pipeIdLast;

protected:
	AbstractInput* input;
	AbstractOutput* output;
	AbstractOutput* errorOutput;
	vector<int> childrenPids;
	Kernel* kernel;
	int pid;
	int parentPid;
	vector<string> parameters;
	void SetParameters(string parameters);
	string path;

public:
	AbstractProcess(int pid, int parentPid, Kernel* kernel);
	~AbstractProcess();
	void Close();
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
	void Join();
	void SetThread(thread* t);
	int GetPipeIdLast();
	void SetPipeIdLast(int pipeIdLast);
};
