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
	bool success = true;
	char c = GetKernel()->ReadFromPipe(pipeIndex, success);
	if (!success)
	{
		Close();
	}
	string s(1, c);
	return s;
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
			//cout << "closing pipe\n";
			Close();
		}
		else if (c != '\n')
		{
			//cout << "reading " << c << " from pipe\n";
			line << c;
		}

		if (!success || c == '\n')
		{
			endLoop = true;
		}		

	}

	return line.str();
}