#include "stdafx.h"
using namespace std;

enum Program { CD, DIR, RAND, SORT, MD, RD, TYPE, FREQ, WC, ECHO };

unordered_map<string, Program> PROGRAM_NAMES =
{
	{ "cd", CD },
	{ "dir", DIR },
	{ "rand", RAND },
	{ "sort", SORT },
	{ "md", MD },
	{ "rd", RD },
	{ "type", TYPE },
	{ "freq", FREQ },
	{ "wc", WC },
	{ "echo", ECHO }
};


Kernel::Kernel() : pipeCounter (0), pidCounter(0) {}

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
	pipe->CloseEntry();
}

void Kernel::ClosePipeOutput(int pipeIndex)
{
	Pipe* pipe = GetPipe(pipeIndex);
	pipe->CloseExit();
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
	AbstractProcess* process = CreateProcessClass(programName, parentPid);
	if (process == NULL)
	{
		return ERROR_UNKNOWN_COMMAND; // unknown command
	}

	AbstractInput* input = CreateInputClass(inputType, inputParam, parentPid);
	AbstractOutput* output = CreateOutputClass(outputType, outputParam, parentPid);
	process->Init(input, output, new StandardOutput(this), parameters);

	
	return process->GetPid();
}

AbstractInput* Kernel::CreateInputClass(IOType type, string param, int parentPid)
{
	AbstractInput* input;
	int pipeId;
	switch (type)
	{
	case STANDARD_TYPE:
		input = new StandardInput(this);
		break;
	case FILE_TYPE:
		input = new FileInput(ifstream(param), this);
		break;
	case PIPE_SINGLE_TYPE:
		pipeId = CreatePipe(true, false, parentPid);
		input = new PipeInput(pipeId, this);		
		break;
	case PIPE_BOTH_TYPE:
		// find proper pipe		
		pipeId = processMap[parentPid]->GetPipeIdLast();
		input = new PipeInput(pipeId, this);
		break;
	default:
		input = NULL;
		break;
	}
	return input;
}

AbstractOutput* Kernel::CreateOutputClass(IOType type, string param, int parentPid)
{
	AbstractOutput* output;
	int pipeId;
	switch (type)
	{
	case STANDARD_TYPE:
		output = new StandardOutput(this);
		break;
	case FILE_TYPE:
		output = new FileOutput(ofstream(param), this);
		break;
	case PIPE_SINGLE_TYPE:
		pipeId = CreatePipe(false, true, parentPid);
		output = new PipeOutput(pipeId, this);
		break;
	case PIPE_BOTH_TYPE:
		pipeId = CreatePipe(false, false, parentPid);
		output = new PipeOutput(pipeId, this);
		break;
	default:
		output = NULL;
		break;
	}
	return output;

	return NULL;
}

int Kernel::CreatePipe(bool closedEntry, bool closedExit, int parentPid)
{
	unique_lock<mutex> locker(pipeMutex);
	int pipeId = ++pipeCounter;
	Pipe* pipe = new Pipe();
	pipeMap[pipeId] = pipe;
	if (closedEntry)
	{
		pipe->CloseEntry();
	}
	if (closedExit)
	{
		pipe->CloseExit();
	}

	locker.unlock();
	
	if (!closedEntry && !closedExit)
	{
		//register new pipe for further program
		processMap[parentPid]->SetPipeIdLast(pipeId);
	}
	else
	{
		// remove any pipe from controller
		processMap[parentPid]->SetPipeIdLast(-1);
	}
	return pipeId;
}

AbstractProcess* Kernel::CreateProcessClass(string programName, int parentPid)
{
	AbstractProcess* process;
	switch (PROGRAM_NAMES[programName])
	{
	case RAND:
		process = new Rand(++pidCounter, parentPid, this);
		break;
	case SORT:
		process = new Sort(++pidCounter, parentPid, this);
		break;
	case CD:
		process = new ChangeDirectory(++pidCounter, parentPid, this);
		break;
	case DIR:
		process = new Dir(++pidCounter, parentPid, this);
		break;
	case MD:
		return NULL;
		break;
	case RD:
		return NULL;
		break;
	case WC:
		return NULL;
		break;
	case TYPE:
		return NULL;
		break;
	case ECHO:
		return NULL;
		break;
	case FREQ:
		return NULL;
		break;
	default:
		return NULL; // unknown command
	}
	
}

int Kernel::WaitForChildren(vector<int>& childrenPids)
{
	for (vector<int>::iterator iterator = childrenPids.begin(); iterator != childrenPids.end(); ++iterator)
	{
		// waiting for child to finish its run
		processMap[*iterator]->Join();
		unique_lock<mutex> locker(mutexProcess);
		// calling destructors
		map<int, AbstractProcess*>::iterator itr = processMap.find(*iterator);
		if (itr != processMap.end())
		{
			delete itr->second;
			processMap.erase(itr->first);
		}		
		locker.unlock();
	}
	return 0;
}
