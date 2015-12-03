#include "stdafx.h"

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

int MakeDirectory::processDirs(string drive, vector<string> sepDirs) {
	int result = 0;
	string tmp_path = drive;
	string tmp_parent;
	for (std::vector<string>::reverse_iterator it = sepDirs.rbegin(); it != sepDirs.rend(); ++it) {		
		tmp_parent = tmp_path;
		tmp_path += '/' + *it;
		int response = 0;
		File* tmpParentFile = kernel->GetFile(tmp_parent, this->GetPathFile(), response);
		File* tmp = kernel->GetFile(tmp_path, this->GetPathFile(), response);
		if (!response) {
			int response = 0;
			kernel->CreateNewFile(*it, FOLDER_ATT, tmpParentFile,response);
			if (response != 0) {
				result = response;
				return result;
			}
		}
	}

	return result;
}

int MakeDirectory::RunProcess()
{
	int result = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}	
	else {
		string drive = kernel->SplitPath(parameters[0], "drive");
		string currentDir = kernel->SplitPath(parameters[0], "filename");		
		string parentDir = kernel->SplitPath(parameters[0], "dir");
		string tmp_path = "";
		vector<string> sepDirs;
		while (currentDir != "") {
			sepDirs.push_back(currentDir);			
			tmp_path = drive + parentDir.substr(0, (parentDir.size()-1));
			currentDir = kernel->SplitPath(tmp_path, "filename");
			parentDir = kernel->SplitPath(tmp_path, "dir");
		}

		result = processDirs(drive, sepDirs);

			
	}

	return result;
};