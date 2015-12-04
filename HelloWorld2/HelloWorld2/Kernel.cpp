#include "stdafx.h"
using namespace std;

enum Program { CD, DIR, RAND, SORT, MD, RD, TYPE, FREQ, WC, ECHO, SHELL };

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
	{ "echo", ECHO },
	{ "shell", SHELL }
};


Kernel::Kernel() : pipeCounter (0), pidCounter(0) {}

string Kernel::ReadLineFromKeyboard(bool& success)
{
	string result = ReadLine(cin, success);
	cin.clear();
	return result;
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
	cin.clear();
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

int Kernel::WriteToFile(FileOutput* output, string content)
{
	shared_ptr<File> file = output->GetFile();
	return file->Write(output, content);
}

int Kernel::WriteLineToFile(FileOutput* output, string content)
{
	return WriteToFile(output, content + "\n");
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

char Kernel::ReadFromFile(FileInput* input, bool& success)
{
	shared_ptr<File> file = input->GetFile();
	return file->ReadChar(input, success);
}

string Kernel::ReadLineFromFile(FileInput* input, bool& success)
{	
	stringstream line;
	char c;
	bool endLoop = false;


	while (!endLoop)
	{
		c = ReadFromFile(input, success);
		if (!success)
		{
			// Close - but it will be handled in input itself			
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
	
BOOL Kernel::QueryLowMemoryStatus() {
	PBOOL lowMemory = NULL;
	QueryMemoryResourceNotification(CreateMemoryResourceNotification(LowMemoryResourceNotification), lowMemory);

	return (bool)lowMemory;
}

int Kernel::Execute(int parentPid, File* pathFile, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam)
{
	AbstractProcess* process = CreateProcessClass(programName, parentPid);
	if (process == NULL)
	{
		return ERROR_UNKNOWN_COMMAND; // unknown command
	}
	processMap[process->GetPid()] = process;
	int response = 0;
	AbstractInput* input = CreateInputClass(inputType, inputParam, parentPid, pathFile, response);
	// TODO handling error
	response = 0;
	AbstractOutput* output = CreateOutputClass(outputType, outputParam, parentPid, pathFile, response);
	// TODO handling error


	process->Init(input, output, new StandardOutput(this), parameters);
	process->SetPathFile(pathFile);
	int returnValue = process->Run();
	if (returnValue == 0)
	{
		return process->GetPid();
	}
	else
	{
		return returnValue;
	}
	
}

AbstractInput* Kernel::CreateInputClass(IOType type, string param, int parentPid, File* pathFile, int& response)
{
	AbstractInput* input;
	int pipeId;
	shared_ptr<File> file;
	switch (type)
	{
	case STANDARD_TYPE:
		input = new StandardInput(this);
		break;
	case FILE_TYPE:
		file = shared_ptr<File>(fileSystem->GetFile(param, pathFile, response));
		if (response == 0)
		{
			if (file->IsFolder())
			{
				response = -10; // file is folder;
				input = NULL;				
			}
			else
			{
				input = new FileInput(file, this);
				response = 0;
			}
		}
		else
		{
			input = NULL;
		}
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

File* Kernel::PrepareFileOutput(string path, File* sourceFile, int& response)
{
	if (path.back() == FILE_SEPARATOR)
	{
		// TODO error handling code
		response = -17;
		return NULL;
	}
	vector<string> pathElements = Utils::Split(path, FILE_SEPARATOR);
	if (pathElements.size() == 0)
	{
		response = -15; // TODO error code
		return NULL;
	}
	else
	{
		// try, if the file already exists
		File* file = fileSystem->GetFile(path, sourceFile, response);
		if (response == 0)
		{
			return file;
		}
	}
	if (pathElements.size() == 1)
	{		
		File* file = this->CreateNewFile(pathElements[0], FILE_ATT, sourceFile, response);
		if (response == 0)
		{
			return file;
		}
		else
		{
			return NULL;
		}
	}
	// it should be found - split worked
	size_t index = path.find_last_of(FILE_SEPARATOR, 500);
	string folderPath = path.substr(0, index);
	string fileName = path.substr(index + 1);
	int tmpResponse = 0;
	File* parent = GetFile(folderPath, sourceFile, tmpResponse);
	if (tmpResponse == 0)
	{
		return CreateNewFile(fileName, FILE_ATT, parent, tmpResponse);
	}
	else
	{
		int pid = Execute(0, sourceFile, "md", "'" + folderPath + "'", STANDARD_TYPE, "", STANDARD_TYPE, "");
		if (pid > 0) 
		{
			vector<int> vectorPid;
			vectorPid.push_back(pid);
			WaitForChildren(vectorPid);
		}
		File* parent = GetFile(folderPath, sourceFile, tmpResponse);
		if (tmpResponse == 0)
		{
			return CreateNewFile(fileName, FILE_ATT, parent, response);
		}
		else
		{
			response = tmpResponse;
			return NULL;
		}
	}
}

AbstractOutput* Kernel::CreateOutputClass(IOType type, string param, int parentPid, File* pathFile, int& response)
{
	AbstractOutput* output;
	int pipeId;
	shared_ptr<File> file;
	switch (type)
	{
	case STANDARD_TYPE:
		output = new StandardOutput(this);
		break;
	case FILE_TYPE:
		
		file = shared_ptr<File>(PrepareFileOutput(param, pathFile, response));
		if (response == 0)
		{
			output = new FileOutput(file, this);
		}
		else
		{
			response = -18;
			output = NULL;
		}
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
			process = new Remove(++pidCounter, parentPid, this);
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
		case SHELL:
			// Create shell only if parentPid = 0 - only one initial shell should be running
			if (parentPid == 0)
			{
				process = new Shell(++pidCounter, parentPid, this);
				break;
			}
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

File* Kernel::LoadFileSystem()
{
	fileSystem = new FileSystem();
	int response = 0;
	File* drive = fileSystem->CreateNewFile("c", FOLDER_ATT, NULL, response);
	return drive;
}

File* Kernel::GetFile(string path, File* sourceFile, int& response)
{
	return fileSystem->GetFile(path, sourceFile, response);
}

File* Kernel::CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response)
{
	return fileSystem->CreateNewFile(name, fileAttribute, parent, response);
}

int Kernel::RemoveFile(string path)
{
	return fileSystem->RemoveFile(path);
}

int Kernel::RemoveFile(File* file)
{
	return fileSystem->RemoveFile(file);
}

bool Kernel::UpdateProcessPathFile(int pid, File* newFilePath)
{
	if (processMap.find(pid) != processMap.end())
	{
		processMap[pid]->SetPathFile(newFilePath);
		return true;
	}
	return false;
}
