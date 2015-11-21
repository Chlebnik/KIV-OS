#pragma once
#include "stdafx.h"
using namespace std;

class FileInput : public AbstractInput
{
	using AbstractInput::AbstractInput;


private:
	ifstream& inputFile;
	bool closed;

public:
	FileInput(ifstream& inputFile, Kernel* kernel);
	int Close();
	bool HasNext();
	string Read();
	string ReadLine(bool& s);
};