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
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;	
	}else{
		for (vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (i == 0 && *it == "--help" && parameters.size() == 1) {
				showHelp = true;
				valid = true;
			}
			else if (i == 0 && *it == "/D" && parameters.size() < 3) {
				changeDrive = true;
				valid = true;
			}
			else if (i == 0 && parameters.size() == 1) {
				pathIndex = i;
				changeDrive = false;
				showHelp = false;
				valid = true;
			}
			else if (i == 1 && parameters.size() == 2){
				pathIndex = i;
				showHelp = false;
				valid = true;
			}
			i++;
		}
	}

	return valid;
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
