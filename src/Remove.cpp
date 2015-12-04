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

bool Remove::RemoveFile(File* file) 
{
	
	// case, when trying delete Shells current folder	
	if (kernel->CheckProcessPath(parentPid, file)) {
		return false;
	}
	// case, when we can immediately remove the file
	if (file->IsDeletable())
	{
		int response = kernel->RemoveFile(file);
		if (response == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// case when simple file (not folder is processed)
	// it cannot be deleted
	if (!file->IsFolder())
	{
		// it would have been already deleted
		return false;
	}
	else
	{
		vector<File*> children = file->GetChildren();
		bool success = true;
		for (vector<File*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
		{
			success = RemoveFile(*iterator);
			if (!success)
			{
				return false;
			}
		}
		return RemoveFile(file);
	}
}

int Remove::RunProcess()
{
	returnVal = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else {	
		int response = 0;
		File* file = kernel->GetFile(parameters[pathIndex], this->GetPathFile(), response);
		if (response == 0) {
			returnVal = RemoveFile(file);			
		}
		else {
			returnVal = response;
		}
		
	}

	this->Close();
	return returnVal;
	
}
