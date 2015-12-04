#include "stdafx.h"

string Echo::GetHelpContent()
{
	string help = "Displays messages, or turns command-echoing on or off.\n\nECHO [ON | OFF]\nECHO[message]\n\nType ECHO without parameters to display the current echo setting.";
	return help;
}

bool Echo::HasValidParameters()
{
	bool valid = false;
	showStatus = false;
	showHelp = false;

	if (parameters.size() == 0) {
		valid = true;
		showStatus = true;
	}
	else if (parameters.size() == 1){
		valid = true;
		if (parameters[0] == "--help") {			
			showHelp = true;
		}else if(parameters[0] == "OFF") {			
			setEchoStatus = false;
		}
		else if (parameters[0] == "ON") {			
			setEchoStatus = true;
		}
	}

	return valid;
}

int Echo::RunProcess() {
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else if (showStatus) {
		int succes;
		bool status = kernel->CheckEchoStatus(this->parentPid, succes);
		if (succes == 0) {
			string result = "OFF";
			if (status) {
				result = "ON";
			}
			output->WriteLine("ECHO is " + result);
		}
		else {
			return -1;
		}
	}
	else if (setEchoStatus != -1) {
		bool succes = kernel->SetEchoStatus(this->parentPid, setEchoStatus);
		if (!succes) {
			return -1;
		}
	}
	else {
		output->WriteLine(parameters[0]);
	}
	output->Close();
	return 0;
}