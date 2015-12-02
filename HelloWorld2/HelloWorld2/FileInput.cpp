#include "stdafx.h"
using namespace std;

FileInput::FileInput(shared_ptr<ifstream> inputFile, shared_ptr<Kernel> kernel) : AbstractInput(kernel), inputFile{ inputFile }, closed(false)
{	
	
}

int FileInput::Close()
{
	inputFile->close();
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
	string line = GetKernel()->ReadLineFromFile(*inputFile, success);
	
	closed = !success;
	return line;	
}