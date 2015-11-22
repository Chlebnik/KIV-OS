#pragma once
#include "stdafx.h"

#define EOF 26

using namespace std;

class Kernel
{	

private:
	string Read(istream& stream);
	string ReadLine(istream& stream, bool& success);
	int Write(ostream& stream, string output);
	int WriteLine(ostream& stream, string output);
	map<int, Pipe*> pipeMap;
	int pipeCounter;
	Pipe* GetPipe(int pipeIndex);

public:
	Kernel();
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


	string WcharToString(wchar_t* text);
	wchar_t* StringToWchar(string text);

	//For fileSystem manipulation
	DWORD OurGetFileAttributesA(const string& dirName_in);
	DWORD OurGetCurrentDirectory(_In_ DWORD BUFSIZE, _Out_ LPWSTR Buffer);
	HANDLE OurFindFirstFile(_In_ LPWSTR szDir, _Out_ LPWIN32_FIND_DATAW ffd);
	DWORD OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd);
};
