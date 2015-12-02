#include "stdafx.h"

void AbstractProcess::setParameters(string parameters)
{
	vector<string> params;
	bool comma = false;
	int startPos = 0;
	int curentPos = 0;
	for (char& c : parameters) {
		if (c == '"' || c == '\'') {
			if (comma) {
				comma = false;
			}
			else {
				comma = true;
			}			
		}else if(isspace(c) && !comma && startPos != (curentPos + 1)){
			string new_param = parameters.substr(startPos,(curentPos-startPos));
			params.push_back(new_param);
			startPos = curentPos + 1;
		}
		else if (curentPos == 0) {
			startPos = curentPos;
		}
		curentPos++;
	}
	if (curentPos != startPos) {
		string new_param = parameters.substr(startPos, (curentPos - startPos));
		params.push_back(new_param);
	}

	this->parameters = params;
}

AbstractProcess::AbstractProcess(int pid, int parentPid, shared_ptr<Kernel> kernel) : pid{ pid }, parentPid{ pid }, kernel{ kernel }, pipeIdLast{ -1 }
{
}

AbstractProcess::~AbstractProcess() {
	input->Close();
	errorOutput->Close();
	output->Close();

}
void AbstractProcess::Init(shared_ptr<AbstractInput> input, shared_ptr<AbstractOutput> output, shared_ptr<AbstractOutput> errorOutput, string parameters)
{
	this->input = input;
	this->output = output;
	this->errorOutput = errorOutput;
	setParameters(parameters);
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

void AbstractProcess::Join()
{
	t->join();
}

void AbstractProcess::SetThread(thread* t)
{
	this->t = t;
}

void AbstractProcess::SetPipeIdLast(int pipeIdLast)
{
	this->pipeIdLast = pipeIdLast;
}

int AbstractProcess::GetPipeIdLast()
{
	return pipeIdLast;
}

