#include "stdafx.h"
using namespace std;

FileInput::FileInput(shared_ptr<File> inputFile, Kernel* kernel) : AbstractInput(kernel), inputFile{ inputFile }, closed(false)
{	
	inputFile->OpenReader(this);
}

int FileInput::Close()
{
	inputFile->CloseReader(this);
	closed = true;
	return 0;
}

bool FileInput::HasNext()
{
	return !closed;
}
string FileInput::Read()
{
	bool success = true;
	return GetKernel()->ReadFromFile(this, success) + "";
}
string FileInput::ReadLine(bool& success)
{
	string line = GetKernel()->ReadLineFromFile(this, success);
	if (!success)
	{
		Close();
	}	
	return line;	
}

shared_ptr<File> FileInput::GetFile()
{
	return inputFile;
}