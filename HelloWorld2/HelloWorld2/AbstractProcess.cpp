#include "stdafx.h"

AbstractProcess::AbstractProcess(int pid, int parentPid, Kernel* kernel) : pid {pid} , parentPid {pid} , kernel {kernel}
{
}

void AbstractProcess::Init(AbstractInput* input, AbstractOutput* output, AbstractOutput* errorOutput, string parameters)
{
	this->input = input;
	this->output = output;
	this->errorOutput = errorOutput;
	this->parameters = parameters;
}

int AbstractProcess::Run()
{
	if (this->HasValidParameters())
	{
		return this->RunProcess();
	}
	else
	{
		return 10;
	}
}

void AbstractProcess::WriteHelp()
{
	output->WriteLine(this->GetHelpContent());
}

void AbstractProcess::SetPath(string path)
{
	this->path = path;
}

int AbstractProcess::GetPid()
{
	return pid;
}

string AbstractProcess::GetPath()
{
	return path;
}

int AbstractProcess::GetParentPid()
{
	return parentPid;
}

