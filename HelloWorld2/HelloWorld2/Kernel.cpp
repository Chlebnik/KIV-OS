#include "stdafx.h"
using namespace std;
Kernel::Kernel()
{
	// TODO remove pipe
	pipeMap[1] = new Pipe();
}

string Kernel::ReadLineFromKeyboard(bool& success)
{
	return ReadLine(cin, success);
}

string Kernel::ReadFromKeyboard()
{
	string strBuf;
		
	char tempChar;
	cin.clear();
	while (cin) 
	{
		cin.get(tempChar);
		if (tempChar == EOF)
		{
			return strBuf;
		}
		strBuf += tempChar;
	}
	cin.ignore();
	return strBuf;	
}

int Kernel::WriteOnMonitor(string output)
{
	return Write(cout, output);
}

int Kernel::WriteLineOnMonitor(string output)
{
	return WriteLine(cout, output);
}

int Kernel::WriteToFile(ofstream& stream, string output)
{
	return Write(stream, output);
}

int Kernel::WriteLineToFile(ofstream& stream, string output)
{
	return WriteLine(stream, output);	
}

int Kernel::Write(ostream& stream, string output)
{
	stream << output;
	return 0;
}

int Kernel::WriteLine(ostream& stream, string output)
{
	stream << output << endl;
	return 0;
}

string Kernel::ReadLine(istream& stream, bool& success)
{
	string line;
	if (getline(stream, line))
	{
		success = true;
		return line;
	}
	success = false;
	return "";
}

string Kernel::ReadLineFromFile(ifstream& stream, bool& success)
{	
	return ReadLine(stream, success);
}

char Kernel::ReadFromPipe(int pipeIndex, bool& success)
{
	char returnValue = EOF;
	
	Pipe* pipe = GetPipe(pipeIndex);
	returnValue = pipe->PopChar(success);
	return returnValue;
}

int Kernel::WriteToPipe(int pipeIndex, char c)
{

	Pipe* pipe = GetPipe(pipeIndex);
	pipe->PushChar(c);
	
	return 0;
}

void Kernel::ClosePipeInput(int pipeIndex)
{
	Pipe* pipe = GetPipe(pipeIndex);
	pipe->CloseInputSide();
}

void Kernel::ClosePipeOutput(int pipeIndex)
{
	Pipe* pipe = GetPipe(pipeIndex);
	pipe->CloseOutputSide();
}

Pipe* Kernel::GetPipe(int pipeIndex)
{
	map<int, Pipe*>::iterator it = pipeMap.find(pipeIndex);
	if (it != pipeMap.end())
	{
		Pipe* pipe = (*it).second;
		return pipe;
	}
	return NULL;
}