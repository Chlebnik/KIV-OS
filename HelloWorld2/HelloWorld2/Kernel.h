#pragma once
#include "stdafx.h"
#include "AbstractProcess.h"

using namespace std;
#define EOF 26
enum IOType {PIPE_SINGLE_TYPE, PIPE_BOTH_TYPE, FILE_TYPE, STANDARD_TYPE};

class Kernel
{	

private:
	
	map<int, Pipe*> pipeMap;
	atomic_int pipeCounter;
	map<int, AbstractProcess*> processMap;
	atomic_int pidCounter;
	mutex mutexProcess;
	mutex pipeMutex;
	
	string Read(istream& stream);
	string ReadLine(istream& stream, bool& success);
	int Write(ostream& stream, string output);
	int WriteLine(ostream& stream, string output);
	
	Pipe* GetPipe(int pipeIndex);
	AbstractProcess* CreateProcessClass(string programName, int parentPid);
	int CreatePipe(bool closedEntry, bool closedExit, int parentPid);
	File* PrepareFileOutput(string path, File* sourceFile, int& response);

public:
	Kernel();
	AbstractInput* CreateInputClass(IOType type, string param, int parendPid, File* pathFile, int& response);
	AbstractOutput* CreateOutputClass(IOType type, string param, int parendPid, File* pathFile, int& response);
	FileSystem* fileSystem;
	//int PrintToMonitor(string output);
	char ReadFromKeyboard(bool& success);
	string ReadLineFromKeyboard(bool& success);
	int WriteOnMonitor(string output);
	int WriteLineOnMonitor(string output);


	int WriteToFile(FileOutput* fileOutput, string content);
	int WriteLineToFile(FileOutput* fileOutput, string content);
	char ReadFromFile(FileInput* fileInput, bool& success);
	string ReadLineFromFile(FileInput* fileInput, bool& success);
	File* LoadFileSystem();

	int WriteToPipe(int pipeIndex, char c);
	char ReadFromPipe(int pipeIndex, bool& success);

	void ClosePipeInput(int pipeIndex);
	void ClosePipeOutput(int pipeIndex);

	int Execute(int parentPid, File* pathFile, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam);
	int WaitForChildren(vector<int>& childrenPids);
	bool UpdateProcessPathFile(int pid, File* newFilePath);
	bool CheckProcessPath(int pid, File* newFilePath);

	//For fileSystem manipulation
	string SplitPath(_In_ string path, _In_ string get);

	File* GetFile(string path, File* sourceFile, int& response);
	File* CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response);
	int RemoveFile(string path);
	int RemoveFile(File* file);

	BOOL QueryLowMemoryStatus();
};
