#pragma once
#include "stdafx.h"
using namespace std;

class FileInput : public AbstractInput
{
	using AbstractInput::AbstractInput;


private:
	shared_ptr<File> inputFile;
	bool closed;

public:
	FileInput(shared_ptr<File> inputFile, Kernel* kernel);
	int Close();
	bool HasNext();
	string Read();
	string ReadLine(bool& s);
	shared_ptr<File> GetFile();
};