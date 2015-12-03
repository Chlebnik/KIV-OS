#include "stdafx.h"

void AbstractProcess::SetParameters(string parameters)
{
	vector<string> params;
	bool comma = false;
	int startPos = 0;
	int curentPos = 0;
	for (char& c : parameters) {
		if (c == '\'') {
			if (comma) {
				comma = false;
			}
			else {
				comma = true;
			}			
		}else if(isspace(c) && !comma && startPos != (curentPos + 1)){
			string new_param = parameters.substr(startPos + 1,(curentPos-startPos - 2));
			params.push_back(new_param);
			startPos = curentPos + 1;
		}
		else if (curentPos == 0) {
			startPos = curentPos;
		}
		curentPos++;
	}
	if (curentPos != startPos) {
		string new_param = parameters.substr(startPos + 1, (curentPos - startPos - 2));
		params.push_back(new_param);
	}

	this->parameters = params;
}

AbstractProcess::AbstractProcess(int pid, int parentPid, Kernel* kernel) : pid{ pid }, parentPid{ parentPid }, kernel{ kernel }, pipeIdLast{ -1 }
{
}

AbstractProcess::~AbstractProcess(){
	delete(input);
	delete(output);
	delete(errorOutput);
}

void AbstractProcess::Close() {
	input->Close();
	errorOutput->Close();
	output->Close();
}
void AbstractProcess::Init(AbstractInput* input, AbstractOutput* output, AbstractOutput* errorOutput, string parameters)
{
	this->input = input;
	this->output = output;
	this->errorOutput = errorOutput;
	SetParameters(parameters);
}

int AbstractProcess::Run()
{
	if (this->HasValidParameters())
	{
		 int value = this->RunProcess();
		 this->Close();
		 return value;
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

void AbstractProcess::SetPathFile(File* pathFile)
{
	this->pathFile = pathFile;
}

int AbstractProcess::GetPid()
{
	return pid;
}

File* AbstractProcess::GetPathFile()
{
	return pathFile;
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

