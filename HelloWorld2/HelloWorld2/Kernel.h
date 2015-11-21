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
};
