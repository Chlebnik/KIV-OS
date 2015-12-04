#include "stdafx.h"
using namespace std;


FileOutput::FileOutput(shared_ptr<File> outputFile, Kernel* kernel) : AbstractOutput(kernel), outputFile{ outputFile }{
	outputFile->OpenWriter(this);
}

int FileOutput::Write(string s)
{
	return GetKernel()->WriteToFile(this, s);
}
int FileOutput::WriteLine(string s)
{
	return GetKernel()->WriteLineToFile(this, s);
}
int FileOutput::Close()
{
	outputFile->CloseWriter(this);	
	return 0;
}

shared_ptr<File> FileOutput::GetFile()
{
	return outputFile;
}
