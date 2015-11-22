#include "stdafx.h"

string Echo::GetHelpContent()
{
	string help = "Displays messages, or turns command-echoing on or off.\n\nECHO [ON | OFF]\nECHO[message]\n\nType ECHO without parameters to display the current echo setting.";
	return help;
}

bool Echo::HasValidParameters()
{
	bool valid = false;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;

	}
	else if (parameters.size() == 1 && parameters[0] == "--help") {
		valid = true;
		showHelp = true;
	}else if(parameters.size() == 1){
		valid = true;
	}

	return valid;
}

void Echo::write_line(string line) {
	output->WriteLine(line);

}

string Echo::read_line() {
	string line;
	bool success = true;

	line = input->ReadLine(success);

	if (success) {
		return line;
	}
}

int Echo::RunProcess() {
	if (showHelp) {
		write_line(GetHelpContent());
	}
	else {
		write_line(parameters[0]);
	}

	return 0;
}