#include "stdafx.h"
using namespace std;


FileOutput::FileOutput(shared_ptr<ofstream> outputFile, shared_ptr<Kernel> kernel) : AbstractOutput(kernel), outputFile{ outputFile }{}

int FileOutput::Write(string s)
{
	return GetKernel()->WriteToFile(*outputFile, s);
}
int FileOutput::WriteLine(string s)
{
	return GetKernel()->WriteLineToFile(*outputFile, s);
}
int FileOutput::Close()
{
	outputFile->close();	
	return 0;
}
