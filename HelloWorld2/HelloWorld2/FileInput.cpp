#include "stdafx.h"
using namespace std;

FileInput::FileInput(ifstream& inputFile, Kernel* kernel) : AbstractInput(kernel), inputFile{ inputFile }, closed(false)
{	
	
}

int FileInput::Close()
{
	closed = true;
	return 0;
}

bool FileInput::HasNext()
{
	return !closed;
}
string FileInput::Read()
{
	return GetKernel()->ReadFromKeyboard();
}
string FileInput::ReadLine(bool& success)
{
	string line = GetKernel()->ReadLineFromFile(inputFile, success);
	
	closed = !success;
	return line;	
}