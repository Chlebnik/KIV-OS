#include "stdafx.h"

string MakeDirectory::GetHelpContent()
{
	string help = "Creates a directory.\n\nMKDIR [drive:]path\nMD[drive:]path\n\nIf Command Extensions are enabled MKDIR changes as follows :\n\nMKDIR creates any intermediate directories in the path, if needed.\nFor example, assume \a does not exist then :\n\nmkdir \a\b\c\d\n\nis the same as :\n\nmkdir \a\nchdir \a\nmkdir b\nchdir b\nmkdir c\nchdir c\nmkdir d\n\nwhich is what you would have to type if extensions were disabled.";
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
	for (std::vector<string>::reverse_iterator it = sepDirs.rbegin(); it != sepDirs.rend(); ++it) {		
		tmp_path += '/' + *it;
		if (!dirExists(tmp_path)) {
			wchar_t* wchar_dir = Utils::StringToWchar(tmp_path);
			if (CreateDirectory(wchar_dir, NULL) == 0) {
				result = -1;
				return result;
			}
		}
	}

	return result;
}

bool MakeDirectory::dirExists(const string& dirName_in)
{
	DWORD ftyp = kernel->OurGetFileAttributesA(dirName_in);

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
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




	output->Close();
	input->Close();

	return result;
};