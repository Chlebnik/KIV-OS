#include "stdafx.h"

using namespace std;

string Shell::GetHelpContent()
{
	return "Shell does not need any help... :)";
}

bool Shell::HasValidParameters()
{
	return true;
}

int Shell::RunProcess()
{
	string line;
	vector<process_data> commands;
	string path;
	while (true)
	{		
		commands.clear();
		path = GetPathFile()->GetAbsolutePath();
		if (!GetPathFile()->IsRoot())
		{
			path = path.substr(0, path.length() - 1);
		}
		output->Write(path + ">");
		bool success = true;
		line = input->ReadLine(success);
		if(!success )
		{
			break;
		}
		if (line == "exit")
		{
			break;
		}
		int index = 0;
		int return_value = this->parseInput(line, index, &commands);
		if (return_value == 0) {
			ExecuteCommands(commands);
		}
		else if (return_value == -1) {
			output->WriteLine("Wrong syntax");
		}
		else if (return_value == -2) {
			output->WriteLine("Unexpected character '" + string(1, line.at(index)) + "' at position " + to_string(index));
		}
		else if (return_value == -3) {
			output->WriteLine("Wrong syntax. Expected character \"");
		}
	}
	return 0;
}

int Shell::ExecuteCommands(vector<process_data> commands) {
	int return_value = 0;
	File* pathFile = GetPathFile();
	vector<int> child_ids;
	child_ids.clear();
	for (int i = 0; i < (int)commands.size(); i++) {
		process_data process = commands.at(i);
		IOType inputType;
		string inputParam;
		IOType outputType;
		string outputParam;
		if (process.process_input.type == 1) {
			inputType = FILE_TYPE;
			inputParam = process.process_input.path;
		}
		else {
			if (i == 0) {
				inputType = STANDARD_TYPE;
			}
			else {
				// Before pipe must be defined a command.
				process_data previous_process = commands.at(i - 1);
				if (previous_process.process_output.type == 1) {
					inputType = PIPE_SINGLE_TYPE;
				}
				else {
					inputType = PIPE_BOTH_TYPE;
				}
			}
		}

		if (process.process_output.type == 1) {
			outputType = FILE_TYPE;
			outputParam = process.process_output.path;
		}
		else {
			if (i + 1 == (int)commands.size()) {
				outputType = STANDARD_TYPE;
			}
			else {
				// After pipe must be defined a command.
				process_data next_process = commands.at(i + 1);
				if (next_process.process_input.type == 1) {
					outputType = PIPE_SINGLE_TYPE;
				}
				else {
					outputType = PIPE_BOTH_TYPE;
				}
			}
		}
		return_value = kernel->Execute(pid, pathFile, process.process_name, process.process_parameters, inputType, inputParam, outputType, outputParam);
		if (return_value < 0) {
			output->WriteLine("Error");
			break;
		}
		child_ids.push_back(return_value);
	}
	kernel->WaitForChildren(child_ids);
	return 0;
}

int Shell::ProcessInput(string line)
{
	bool insideQuotes = false;
	bool mustFollowSpace = false;
	bool wasSpaceBefore = false;
	string param = "";
	vector<string> parameters {};
	int size = 0;

	for (string::iterator it = line.begin(); it != line.end(); ++it)
	{
		char c = *it;
		
		if (insideQuotes && c != '\"')
		{
			param += c;
			continue;
		}	

		switch (c)
		{
		case ' ':
			if (param.size() > 0)
			{
				parameters.resize(parameters.size() + 1);
				parameters[parameters.size() - 1] = param;
			}
			mustFollowSpace = false;
			wasSpaceBefore = true;
			param = "";
			break;
		case '\"':
			if (insideQuotes)
			{
				mustFollowSpace = false;
				insideQuotes = false;
			}
			else
			{
				if (!wasSpaceBefore && parameters.size() > 0)
				{
					return 12; // space must be preceding
				}
				insideQuotes = true;
			}
			
			break;
		case '|':
		case '>':
		case '<':
			if (!wasSpaceBefore)
			{
				return 12; // space has to be before pipe character
			}
			parameters.resize(parameters.size() + 1);
			parameters[parameters.size() - 1] = "|";
			mustFollowSpace = true;
			break;
		default:
			param += c;
		}

	}

	if (insideQuotes)
	{
		return 13; // Quotes not ended
	}

	if (param.size() > 0)
	{
		parameters.resize(parameters.size() + 1);
		parameters[parameters.size() - 1] = param;
	}

	for (vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		output->WriteLine(*it);
	}
	output->WriteLine("");
	return 0;
}


int Shell::ParseProgram(vector<process_data>* commands, string input, int& index) {
	string program_name = "";
	int return_value;
	process_data process_data;

	while (index < (int)input.length()) {
		char ch = input.at(index);
		if ((ch == SPACE_CHAR)) {
			break;
		}
		else if ((ch == OUTPUT_REDIRECT) || (ch == INPUT_REDIRECT) || (ch == PIPE)) {
			return -1;
		}
		if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9'))) {
			program_name = program_name + ch;
			index++;
		}
		else {			
			return -2;
		}
	}
	if (program_name.length() == 0) {		
		return -1;
	}

	process_data.process_name = program_name;

	return_value = ParseArguments(&process_data, input, index);
	if (return_value != 0) {		
		return return_value;
	}
	commands->push_back(process_data);
	return 0;
}

int Shell::ParseArguments(process_data* command, string input, int& index) {
	if (SkipSpaces(input, index) != 0) {
		return -1;
	}
	string value;
	int return_value;
	while (index < (int)input.length()) {
		value = "";
		return_value = ReadValue(command, value, input, index);
		if (return_value == 1) {
			// Next char is a pipe => parse next command. 			
			return 0;
		}
		else if (return_value == 0) {
			if (command->process_parameters.length() > 0) {
				command->process_parameters = command->process_parameters + " ";
			}
			command->process_parameters = command->process_parameters + "'" + value + "'";
		}
		else if (return_value < 0) {
			return return_value;
		}
	}
	return 0;
}

int Shell::SkipSpaces(string input, int &index) {
	while (index < (int)input.length()) {
		char ch = input.at(index);
		if (ch != SPACE_CHAR)
			break;
		index++;
	}
	return 0;
}

int Shell::ReadValue(process_data* command, string& value, string input, int& index) {
	if (SkipSpaces(input, index) != 0) {
		return -1;
	}
	if (index < (int)input.length()) {
		char ch = input.at(index);
		// If there is control symbol, skip reading value.
		if (ch == PIPE) {
			return 1;
		}
		else if ((ch == OUTPUT_REDIRECT) || (ch == INPUT_REDIRECT)) {
			int return_value = ParseRedirect(command, input, index, ch);
			if (return_value == 0) {
				return 2;
			}
			return return_value;
		}
		if (ch == QUOTATION) {
			index++;
			boolean quatation_expected = true;
			while (index <(int)input.length()) {
				ch = input.at(index);
				index++;
				if (ch == QUOTATION) {
					quatation_expected = false;
					break;
				}
				value = value + ch;
			}
			if (quatation_expected) {
				return -3;
			}				
		}
		else {
			do {
				ch = input.at(index);
				if ((ch == SPACE_CHAR)) {
					break;
				}
				else if ((ch == OUTPUT_REDIRECT) || (ch == INPUT_REDIRECT) || (ch == PIPE)) {
					return -1;
				}
				value = value + ch;
				index++;
			} while (index < (int)input.length());
		}

		if (value.length() > 0) {
		}
	}
	return 0;
}

int Shell::ParseRedirect(process_data* command, string input, int& index, char redirect) {
	if (SkipSpaces(input, index) != 0) {
		return -1;
	}
	if (index < (int)input.length()) {
		char ch = input.at(index);
		if (ch == redirect) {
			if (index + 2 >= (int)input.length() || input.at(index + 1) != SPACE_CHAR) {	
				return -1;
			}
			index++;
			string value = "";
			int return_value = ReadValue(command, value, input, index);
			if (return_value != 0) {
				if (return_value < 0) {
					return return_value;
				}
				return -2;
			}
			if (redirect == INPUT_REDIRECT) {
				process_input input;
				input.type = 1;
				input.path = value;
				command->process_input = input;
			}
			else if (redirect == OUTPUT_REDIRECT) {
				process_output output;
				output.type = 1;
				output.path = value;
				command->process_output = output;
			}
		}
	}
	return 0;
}

int Shell::parseInput(string input, int& index, vector<process_data>* commands) {
	
	int return_value;
	SkipSpaces(input, index);
	if (input.empty() || input.length() == index) {
		return 0;
	}

	return_value = ParseProgram(commands, input, index);
	if (return_value != 0) {
		return return_value;
	}
	while (index < (int)input.length()) {
		char ch = input.at(index);
		if (ch != PIPE) {
			return -1;
		}
		index++;
		if (index >= (int)input.length()) {
			return -1;
		}
		ch = input.at(index);
		if (ch != SPACE_CHAR) {
			return -1;
		}
		SkipSpaces(input, index);
		return_value = ParseProgram(commands, input, index);
		if (return_value != 0) {
			return return_value;
		}
	}

	if (index < (int)input.length()) {
		return -1;
	}
	return 0;
}



