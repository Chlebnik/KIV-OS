#pragma once
#include "stdafx.h"
#include "AbstractProcess.h"

using namespace std;
#define EOF 26
enum IOType {PIPE_SINGLE_TYPE, PIPE_BOTH_TYPE, FILE_TYPE, STANDARD_TYPE};

class Kernel
{	

private:
	string Read(istream& stream);
	string ReadLine(istream& stream, bool& success);
	int Write(ostream& stream, string output);
	int WriteLine(ostream& stream, string output);
	map<int, Pipe*> pipeMap;
	atomic_int pipeCounter;
	map<int, AbstractProcess*> processMap;
	atomic_int pidCounter;
	mutex mutexProcess;
	mutex pipeMutex;
	
	Pipe* GetPipe(int pipeIndex);
	AbstractProcess* CreateProcessClass(string programName, int parentPid);
	AbstractOutput* CreateOutputClass(IOType type, string param, int parentPid);
	int CreatePipe(bool closedEntry, bool closedExit, int parentPid);

public:
	Kernel();
	AbstractInput* CreateInputClass(IOType type, string param, int parendPid);
	//int PrintToMonitor(string output);
	string ReadFromKeyboard();
	string ReadLineFromKeyboard(bool& success);
	int WriteOnMonitor(string output);
	int WriteLineOnMonitor(string output);


	int WriteToFile(ofstream& stream, string output);
	int WriteLineToFile(ofstream& stream, string output);
	//string ReadFromFile(ifstream& stream);
	string ReadLineFromFile(ifstream& stream, bool& success);

	int WriteToPipe(int pipeIndex, char c);
	char ReadFromPipe(int pipeIndex, bool& success);

	void ClosePipeInput(int pipeIndex);
	void ClosePipeOutput(int pipeIndex);

	int Execute(int parentPid, string path, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam);
	int WaitForChildren(vector<int>& childrenPids);	

	//For fileSystem manipulation
	wchar_t* ValidatePath(_In_ string path, _Out_ int result);
	string SplitPath(_In_ string path, _In_ string get);

	DWORD OurGetFileAttributesA(const string& dirName_in);
	DWORD OurGetCurrentDirectory(_In_ DWORD BUFSIZE, _Out_ LPWSTR Buffer);
	HANDLE OurFindFirstFile(_In_ string path, _Out_ LPWIN32_FIND_DATAW ffd);
	DWORD OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd);
	BOOL QueryLowMemoryStatus();
};
