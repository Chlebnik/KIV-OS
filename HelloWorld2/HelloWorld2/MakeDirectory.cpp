#include "stdafx.h"

using namespace std;

string MakeDirectory::GetHelpContent()
{
	string help = "Creates a directory.\n\n"
		"MD[drive:]path\n\n"
		"If Command Extensions are enabled MKDIR changes as follows :\n\n"
		"MD creates any intermediate directories in the path, if needed.\n"
		"For example, assume \a does not exist then :\n\n"
		"md \a\b\c\d\n\n"
		"is the same as :\n\n"
		"md \a\n"
		"cd \a\n"
		"md b\n"
		"cd b\n"
		"md c\n"
		"cd c\n"
		"md d\n\n"
		"which is what you would have to type if extensions were disabled.";
	return help;
}

bool MakeDirectory::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;
	}
	else if (parameters[0] == "--help" && parameters.size() == 1) {
		showHelp = true;
		valid = true;
	}
	else if(parameters.size() == 1) {
		showHelp = false;
		valid = true;
	}

	return valid;
}

int MakeDirectory::RunProcess()
{
	int result = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}	
	else {
		vector<string> pathElements = Utils::Split(parameters[0], FILE_SEPARATOR);

		string element = pathElements[0];
		int response = 0;
		File* f = kernel->GetFile(element, GetPathFile(), response);


		int i = 0;
		if (element.back() == DRIVE_SUFFIX)
		{
			if (response != 0)
			{
				errorOutput->WriteLine("Drive does not exist.");
				return response;
			}
			else
			{
				if (pathElements.size() == 1)
				{
					errorOutput->WriteLine("Error: It is not allowed to create a drive.");
					return 26; // TODO error id
				}
				else
				{
					i = 1;
				}

			}
		}
		else
		{
			f = pathFile;
		}
		File* tmp;
		for (; i < pathElements.size(); i++)
		{
			element = pathElements[i];
			tmp = kernel->GetFile(element, f, response);

			if (response == 0 && i == pathElements.size() - 1)
			{
				errorOutput->WriteLine("Error: File or folder " + element + " already exists.");
				return 25; // TODO error code;
			}

			if (response == 0)
			{
				if (tmp->IsFolder())
				{
					f = tmp;
					continue;
				}
				else
				{
					errorOutput->WriteLine("Error: " + tmp->GetName() + " is not a folder.");
					return -1;
				}
			}
			else
			{
				if (response == 2)
				{
					errorOutput->WriteLine("Error: No parent folder above drive.");
					return response;
				}
				break;
			}
			
		}

		for (int j = i; j < pathElements.size(); j++)
		{
			element = pathElements[j];
			f = kernel->CreateNewFile(element, FOLDER_ATT, f, response);
		}
	}

	return result;
}