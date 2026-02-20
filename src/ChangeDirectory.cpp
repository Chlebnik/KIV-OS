#include "stdafx.h"

using namespace std;

string ChangeDirectory::GetHelpContent()
{
	string help = "Changes the current directory.\n\n"
		"CD[drive:][path]\nCD[..]\n\n"
		"..Specifies that you want to change to the parent directory.\n\n";
	return help;
}

bool ChangeDirectory::HasValidParameters()
{
	showHelp = false;
	changeDrive = false;
	pathIndex = 0;

	if (parameters.empty()) {
		showHelp = true;
		return true;
	}

	if (parameters.size() == 1) {
		if (parameters[0] == "--help") {
			showHelp = true;
			return true;
		}
		if (parameters[0] == "/D") {
			return false;
		}
		pathIndex = 0;
		return true;
	}

	if (parameters.size() == 2) {
		if (parameters[0] == "/D") {
			changeDrive = true;
			pathIndex = 1;
			return true;
		}
		return false;
	}

	return false;
}

int ChangeDirectory::RunProcess()
{
	input->Close();
	int returnValue = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
		output->Close();
	}
	else {
		output->Close();
		returnValue = ChangePath(parameters[pathIndex]);
	}

	return returnValue;
}


int ChangeDirectory::ChangePath(string new_path) {
	int success = 0;
	int response = 0;

	File* file = kernel->GetFile(new_path, this->GetPathFile(), response);

	if (response != 0) {
		return response;
	}
	
	if (file->IsFolder()) {
		
		if (!kernel->UpdateProcessPathFile(parentPid, file)) {
			return -1;
		}

	}
	else {
		//"Given path doesn't exist!"
		success = ERROR_INVALID_FILE_ATTRIBUTES;
	}

	return success;
}
