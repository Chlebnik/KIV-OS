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
	shared_ptr<AbstractInput> input;
	shared_ptr<AbstractOutput> output;
	shared_ptr<AbstractOutput> errorOutput;
	vector<int> childrenPids;
	shared_ptr<Kernel> kernel;
	int pid;
	int parentPid;
	vector<string> parameters;
	void setParameters(string parameters);
	string path;

public:
	AbstractProcess(int pid, int parentPid, shared_ptr<Kernel> kernel);
	~AbstractProcess();
	virtual string GetHelpContent()
	{
		return "This program or command has no help content.";
	}
	virtual bool HasValidParameters() = 0;
	virtual int RunProcess() = 0;
	
	void Init(shared_ptr<AbstractInput> input, shared_ptr<AbstractOutput> output, shared_ptr<AbstractOutput> errorOutput, string parameters);
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
