#pragma once
#include "stdafx.h"
using namespace std;

class FileOutput : public AbstractOutput
{
	using AbstractOutput::AbstractOutput;

private:
	shared_ptr<File> outputFile;

public:
	FileOutput(shared_ptr<File> outputFile, Kernel* kernel);

	int Write(string s);
	int WriteLine(string s);
	int Close();
	shared_ptr<File> GetFile();
};
