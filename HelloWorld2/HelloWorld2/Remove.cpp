#include "stdafx.h"

string Remove::GetHelpContent()
{
	string help = "Syntax\n"
		"RD pathname\n"
		"RD /S pathname\n\n"
		"Key\n"
		"   /S  : Delete all files and subfolders\n"
		"         in addition to the folder itself.\n"
		"         Use this to remove an entire folder tree.";
	return help;
}


bool Remove::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = false;
		pathIndex = -1;
	}
	else {
		for (std::vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (i == 0 && *it == "--help" && parameters.size() == 1) {
				showHelp = true;
				valid = true;
			}
			else if (i == 0 && *it == "/S" && parameters.size() == 2) {
				force = true;
				showHelp = false;
			}
			else if (i == 0 && parameters.size() == 1) {
				showHelp = false;
				force = false;
				valid = true;
				pathIndex = i;

			}
			else if (i == 1 && parameters.size() == 2) {
				pathIndex = i;
				showHelp = false;
			}
			i++;
		}
	}

	return valid;
}

bool Remove::RemoveFile(File* file) {
	if(file->IsDeletable()){
		for (File* child : file->GetChildren()) {
			if (!force) {
				if (file->IsDeletable()) {
					return false;
				}
				if (RemoveFile(child)) {
					file->RemoveChild(child);
					kernel->RemoveFile(child);
				}
				else {
					return false;
				}

			}
		}
		kernel->RemoveFile(file);
		return true;
	}
	return false;
}
int Remove::RunProcess()
{
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else {
		int response = 0;
		File* file = kernel->GetFile(parameters[pathIndex], this->GetPathFile(), response);
		if (response != 0) {
			return response;
		}
		return RemoveFile(file);
	}

	return 0;
}
