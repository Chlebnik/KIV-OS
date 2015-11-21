#include "stdafx.h"
using namespace std;

int StandardInput::Close()
{
	this->closed = false;
	return 0;
}

bool StandardInput::HasNext()
{
	return !closed;
}
string StandardInput::Read()
{
	return GetKernel()->ReadFromKeyboard();
}
string StandardInput::ReadLine(bool& success)
{	
	string line = GetKernel()->ReadLineFromKeyboard(success);
	if (!success)
	{
		closed = true;
		return "";
	}
	if (line.size() > 0)
	{
		string::size_type loc = line.find("\x1A", 0);
		if (loc != string::npos)
		{
			closed = true;
			stringstream streamTmp;
			streamTmp << line;			
			getline(streamTmp, line, '\x1A');			
			return line;
		}
	}
	if (!closed) {
		return line;
	}
	else
	{
		return "";
	}
}
