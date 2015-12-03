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

string Kernel::SplitPath(string path, string get){//, char* drive, char* dir, char* fname, char* ext) {
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	const char* filepath = path.c_str();

	_splitpath_s(filepath, drive, dir, fname, ext);

	if (get == "basename") {
		return string(fname) + string(ext);
	}
	else if (get == "filename") {
		return string(fname);
	}
	else if (get == "dir") {
		return string(dir);
	}
	else if (get == "drive") {
		return string(drive);
	}
	else if (get == "ext") {
		return string(ext);
	}
	else {
		return "";
	}

}

wchar_t* Kernel::ValidatePath(string path, int result) {
	string line;
	TCHAR Buffer[MAX_PATH];
	DWORD dwRet;
	wchar_t* wstr = NULL;

	if (path.size() == 0) {
		dwRet = OurGetCurrentDirectory(MAX_PATH, Buffer);
		if (dwRet == 0)
		{
			//GetCurrentDirectory faile;
			result = -1;
			return wstr;
		}
		if (dwRet > MAX_PATH)
		{
			//line = "Buffer too small; need more characters";
			result = -2;
			return wstr;
		}
		wstr = Buffer;
	}
	else {
		wstr = Utils::StringToWchar(path);
	}


	if (lstrlen(wstr) > (MAX_PATH - 3))
	{
		//output->WriteLine("\nDirectory path is too long.\n");
		result = -3;
		return wstr;
	}

	return wstr;
}

HANDLE Kernel::OurFindFirstFile(_In_ string path, _Out_ LPWIN32_FIND_DATAW ffd) {

	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	int result = 0;
	wchar_t* wstr;

	wstr = ValidatePath(path, result);

	if (result != 0) {
		return hFind;
	}

	lstrcpy(szDir, wstr);
	lstrcat(szDir, TEXT("\\*"));

	hFind = FindFirstFile(szDir, ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		//"Invalid target directory.";
		return hFind;
	}


	return hFind;
}

DWORD Kernel::OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd) {
	return FindNextFile(hFind, ffd);
	//a
}

BOOL Kernel::QueryLowMemoryStatus() {
	PBOOL lowMemory = NULL;
	QueryMemoryResourceNotification(CreateMemoryResourceNotification(LowMemoryResourceNotification), lowMemory);

	return (bool)lowMemory;
}

int Kernel::Execute(int parentPid, string path, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam)
{
	AbstractProcess* process = CreateProcessClass(programName, parentPid);
	if (process == NULL)
	{
		return ERROR_UNKNOWN_COMMAND; // unknown command
	}
	processMap[process->GetPid()] = process;
	AbstractInput* input = CreateInputClass(inputType, inputParam, parentPid);
	AbstractOutput* output = CreateOutputClass(outputType, outputParam, parentPid);

	process->Init(input, output, new StandardOutput(this), parameters);
	int returnValue = process->Run();

	
	return returnValue;
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
		input = new FileInput(shared_ptr<ifstream>(new ifstream(param)), this);
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
		output = new FileOutput(shared_ptr<ofstream>(new ofstream(param)), this);
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
	if (PROGRAM_NAMES.count(programName) == 0) {
		process = NULL;
	}else{
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
			process = new MakeDirectory(++pidCounter, parentPid, this);
			break;
		case RD:
			process = NULL;
			break;
		case WC:
			process = new WordCount(++pidCounter, parentPid, this);
			break;
		case TYPE:
			process = new Type(++pidCounter, parentPid, this);
			break;
		case ECHO:
			process = new Echo(++pidCounter, parentPid, this);
			break;
		case FREQ:
			process = new Freq(++pidCounter, parentPid, this);
			break;
		default:
			process = NULL; // unknown command
		}
	}
	
	return process;
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
