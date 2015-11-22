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


DWORD Kernel::OurGetFileAttributesA(const string& dirName_in) {
	return GetFileAttributesA(dirName_in.c_str());
}

DWORD Kernel::OurGetCurrentDirectory(_In_ DWORD BUFSIZE, _Out_ LPWSTR Buffer) {
	return GetCurrentDirectory(BUFSIZE, Buffer);
}


HANDLE Kernel::OurFindFirstFile(_In_ LPWSTR szDir, _Out_ LPWIN32_FIND_DATAW ffd) {
	return FindFirstFile(szDir, ffd);
}

DWORD Kernel::OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd) {
	return FindNextFile(hFind, ffd);
}

int Kernel::Execute(int parentPid, string path, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam)
{
	
	return 0;
}

int Kernel::WaitForChildren(int parentPid)
{
	return 0;
}
