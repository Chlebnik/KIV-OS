#include "stdafx.h"
using namespace std;

PipeInput::PipeInput(int pipeIndex, Kernel* kernel) : AbstractInput(kernel), pipeIndex{ pipeIndex }, closed(false)
{

}

int PipeInput::Close()
{
	closed = true;
	return 0;
}

bool PipeInput::HasNext()
{
	return !closed;
}

string PipeInput::Read()
{
	return "";
}

string PipeInput::ReadLine(bool &success)
{
	stringstream line;
	char c;
	bool endLoop = false;
	
	
	while (!endLoop)
	{
		c = GetKernel()->ReadFromPipe(pipeIndex, success);
		if (!success)
		{
			Close();
		}
		else if (c != '\n')
		{
			line << c;
		}

		if (!success || c == '\n')
		{
			endLoop = true;
		}		

	}

	return line.str();
}