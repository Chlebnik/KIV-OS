#pragma once
#include "stdafx.h"
using namespace std;

class FileInput : public AbstractInput
{
	using AbstractInput::AbstractInput;


private:
	shared_ptr<ifstream> inputFile;
	bool closed;

public:
	FileInput(shared_ptr<ifstream> inputFile, shared_ptr<Kernel> kernel);
	int Close();
	bool HasNext();
	string Read();
	string ReadLine(bool& s);
};