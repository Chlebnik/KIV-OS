 #pragma once
 #include "stdafx.h"
 #include "Struct.h"
 
 class Shell : public AbstractProcess
	using AbstractProcess::AbstractProcess;
 {
 	string GetHelpContent();
 	bool HasValidParameters();
 	int RunProcess();
	int parseInput(std::string input, std::vector<process_data>* commands);
 
 private:
	static const char SPACE_CHAR = ' ';
	const char QUOTATION = '"';
	const char INPUT_REDIRECT = '<';
	const char OUTPUT_REDIRECT = '>';
	const char PIPE = '|';

	int ParseProgram(vector<process_data>* commands, string input, int& index);
	int ParseArguments(process_data* command, string input, int& index);
	int SkipSpaces(string input, int& index);
	int ReadValue(process_data* command, string& value, string input, int& index);
	int ParseRedirect(process_data* command, string input, int& index, char redirect);
	int ProcessInput(string line);
	int ExecuteCommands(vector<process_data> commands);
 }; 