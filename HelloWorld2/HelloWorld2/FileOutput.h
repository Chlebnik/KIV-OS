#pragma once
#include "stdafx.h"
using namespace std;

class FileOutput : public AbstractOutput
{
	using AbstractOutput::AbstractOutput;

private:
	shared_ptr<ofstream> outputFile;

public:
	FileOutput(shared_ptr<ofstream> outputFile, Kernel* kernel);

	int Write(string s);
	int WriteLine(string s);
	int Close();
};
