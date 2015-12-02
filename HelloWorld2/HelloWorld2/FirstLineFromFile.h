#pragma once
#include "stdafx.h"

class Kernel;
class AbstractInput;

using namespace std;

class FirstLineFromFile
{
private:
	shared_ptr<AbstractInput> input;
	string line;
	int index;

public:
	FirstLineFromFile(int index, string path, shared_ptr<Kernel> kernel);
	~FirstLineFromFile();
	int GetIndex();
	string GetLine(bool& succes);
	bool static FirstOperatorString(string aString, string bString);
	bool static FirstOperator(FirstLineFromFile const & a, FirstLineFromFile const & b);
	bool HasNext();
};
