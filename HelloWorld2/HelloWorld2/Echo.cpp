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

int Echo::RunProcess() {
	input->Close();
	if (showHelp) {
		output->WriteLine(GetHelpContent());		
	}
	else {
		output->WriteLine(parameters[0]);
	}
	output->Close();
	return 0;
}