#include "stdafx.h"
using namespace std;

PipeOutput::PipeOutput(int pipeIndex, Kernel* kernel) : AbstractOutput(kernel), pipeIndex{ pipeIndex }, closed(false)
{

}

int PipeOutput::GetPipeIndex()
{
	return pipeIndex;
}

int PipeOutput::Write(string s)
{
	for (string::iterator it = s.begin(); it != s.end(); ++it) 
	{
		char c = *it;
		GetKernel()->WriteToPipe(pipeIndex, c);
		/*if (c == EOF)
		{
			closed = true;
			// this should not happen inside of string, but to be sure...
			break;
		}*/
	}
	return 0;
}

int PipeOutput::WriteLine(string s)
{
	Write(s);
	Write("\n");
	return 0;
}

int PipeOutput::Close()
{
	GetKernel()->ClosePipeOutput(pipeIndex);
	closed = true;
	return 0;
}
