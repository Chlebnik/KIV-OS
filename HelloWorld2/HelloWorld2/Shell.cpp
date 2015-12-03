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
	while (true)
	{		
		commands.clear();
		output->Write(GetPathFile()->GetAbsolutePath() + ">");
		bool success = true;
		line = input->ReadLine(success);
		if (line == "exit")
		{
			break;
		}
		if (this->parseInput(line, &commands) == 0) {
			ExecuteCommands(commands);
		}
	}
	return 0;
}

int Shell::ExecuteCommands(vector<process_data> commands) {
	int returnValue = 0;
	File* pathFile = GetPathFile();
	for (int i = 0; i < (int)commands.size(); i++) {
		process_data process = commands.at(i);
		string params = "";
		for (int j = 0; j < (int)process.process_parameters.size(); j++) {
			if (j > 0) {
				params = params + " ";
			}
			params = params + "'" + (process.process_parameters.at(j)) + "'";
		}
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
				// TODO jednosmerna pipe
				inputType = PIPE_BOTH_TYPE;
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
				// TODO jednosmerna pipe
				outputType = PIPE_BOTH_TYPE;
			}
		}
		returnValue = kernel->Execute(pid, pathFile, process.process_name, params, inputType, inputParam, outputType, outputParam);
		if (returnValue != 0) {
			output->WriteLine("Error");
		}
	}
	return returnValue;
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
		if ((ch == SPACE_CHAR) || (ch == OUTPUT_REDIRECT) || (ch == INPUT_REDIRECT) || (ch == PIPE)) {
			break;
		}
		if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9'))) {
			program_name = program_name + ch;
			index++;
		}
		else {
			
			output->WriteLine("Unexpected character " + to_string(input.at(index)) + " at position " + to_string(index));
			return -1;
		}
	}
	if (program_name.length() == 0) {
		output->WriteLine("Unexpected character " + to_string(input.at(index)) + " at position " + to_string(index));
		return -1;
	}

	process_data.process_name = program_name;

	return_value = ParseArguments(&process_data, input, index);
	if (return_value != 0) {
		output->WriteLine("Unexpected character " + to_string(input.at(index)) + " at position " + to_string(index));
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
			command->process_parameters.push_back(value);
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
			while (index <(int)input.length()) {
				ch = input.at(index);
				index++;
				if (ch == QUOTATION) {
					break;
				}
				value = value + ch;
			}
			if (index >(int) input.length())
				return -1;
		}
		else {
			do {
				ch = input.at(index);
				if ((ch == SPACE_CHAR) || (ch == OUTPUT_REDIRECT) || (ch == INPUT_REDIRECT) || (ch == PIPE)) {
					break;
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
			if (index + 1 >= (int)input.length()) {
				return -1;
			}
			index++;
			string value = "";
			if (ReadValue(command, value, input, index) != 0) {
				return -1;
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

int Shell::parseInput(string input, vector<process_data>* commands) {
	int index = 0;
	int return_value;
	if (input.empty()) {
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



