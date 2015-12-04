#include "stdafx.h"

using namespace std;

FirstLineFromFile::FirstLineFromFile(int index, string path, Kernel* kernel, int parentPid, File* sourceFile)
{
	bool success;
	int response = 0;
	this->index = index;
	this->input = kernel->CreateInputClass(FILE_TYPE, path, parentPid, sourceFile, response);
	this->line = input->ReadLine(success);
}

FirstLineFromFile::~FirstLineFromFile() {
	//input->Close();
	//delete(input);
}

int FirstLineFromFile::GetIndex() {
	return index;
}

string FirstLineFromFile::GetLine(bool& succes) {
	string tmp_line = this->line;
	this->line = input->ReadLine(succes);

	if (!succes) {
		input->Close();
	}

	return tmp_line;
}

bool FirstLineFromFile::FirstOperator(FirstLineFromFile const & a, FirstLineFromFile const & b) {
	return FirstOperatorString(a.line, b.line);
}

bool FirstLineFromFile::FirstOperatorString(string aString, string bString)
{
	
	if (aString.size() != bString.size()) {
		return  aString.size() < bString.size();
	}
	else {
		for (int i = 0; i < aString.size(); i++) {
			if (aString.at(i) != bString.at(i)) {
				return aString.at(i) < bString.at(i);
			}
		}
	}
	
	return aString < bString;
}

bool FirstLineFromFile::HasNext() {
	return input->HasNext();
}