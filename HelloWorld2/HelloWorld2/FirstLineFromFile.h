#pragma once
#include "stdafx.h"

class Kernel;
class AbstractInput;

using namespace std;

class File;

class FirstLineFromFile
{
private:
	AbstractInput* input;
	string line;
	int index;

public:
	FirstLineFromFile(int index, string path, Kernel* kernel, int parentPid, File* sourceFile);
	~FirstLineFromFile();
	int GetIndex();
	string GetLine(bool& succes);
	bool static FirstOperatorString(string aString, string bString);
	bool static FirstOperator(FirstLineFromFile const & a, FirstLineFromFile const & b);
	bool HasNext();
};
