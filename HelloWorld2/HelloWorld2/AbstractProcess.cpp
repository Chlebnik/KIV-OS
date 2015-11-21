#include "stdafx.h"

AbstractProcess::AbstractProcess(int pid, Kernel* kernel)
{
	this->pid = pid;
	this->kernel = kernel;
}

void AbstractProcess::Init(AbstractInput* input, AbstractOutput* output, AbstractOutput* errorOutput, vector<string> parameters)
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