#include "stdafx.h"

string Type::GetHelpContent()
{
	string help = "Display the contents of one or more text files.\n\n		Syntax\n		TYPE[drive:]pathname(s)";
	return help;
}

bool Type::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;
	} 
	else if(parameters[0] == "--help" && parameters.size() == 1){
		showHelp = true;
		valid = true;
	}
	else {
		fromFile = true;
		showHelp = false;
		valid = true;
	}

	return valid;
}

int Type::printFileContent(AbstractInput * printInput)
{
	int returnValue = 0;

	bool success = false;
	string line = "";
	while (printInput->HasNext()) {
		line = printInput->ReadLine(success);
		if (success) {
			output->WriteLine(line);
		}
		else {
			returnValue = -5;
			break;
		}
		
	}
	return returnValue;
}

int Type::proccesFile(string filepath) {
	int returnValue = 0;
	string basename = kernel->SplitPath(filepath, "basename");
	AbstractInput* fileFromParam = kernel->CreateInputClass(FILE_TYPE, filepath, this->GetPid());

	output->WriteLine("\n" + basename + "\n");
	returnValue = printFileContent(fileFromParam);
	fileFromParam->Close();

	return returnValue;
}

int Type::listDir(string path, string regexFile) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	int returnValue = 0;

//	hFind = kernel->OurFindFirstFile(path, &ffd);
	/*
	// List all the files in the directory with some info about them.
	do
	{
		string line;
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			string fileName = Utils::WcharToString(ffd.cFileName);
			regex findFiles = regex(regexFile);
			if (regex_match(fileName, findFiles)) {
				string filePath = path + "/" +  fileName;
				returnValue = proccesFile(filePath);
				if (returnValue != 0) {
					return returnValue;
				}
			}
		}

	} while (true);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		output->WriteLine("FindFirstFile");
		FindClose(hFind);
	}
	*/
	return returnValue;

}

int Type::RunProcess()
{
	int returnValue = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else if (fromFile) {
		for (std::vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			string filepath = *it;
			
			if (filepath.find("*") != (filepath.npos)) {
				string tmp_regex = filepath.substr((filepath.rfind('/')+1), filepath.size());
				string regexPattern = "";
				if (tmp_regex.find("*") > 0) {
					regexPattern = "(" + tmp_regex.substr(0, tmp_regex.find("*")) + ")";
				}
				regexPattern += "(.*)";
				if (tmp_regex.find("*") < tmp_regex.size()) {
					regexPattern += "(" + tmp_regex.substr((tmp_regex.find("*") + 1), tmp_regex.size()) + ")";
				}
				
				string dirPath = filepath.substr(0, filepath.rfind('/'));
				returnValue = listDir(dirPath, regexPattern);
			}
			else {
				returnValue = proccesFile(filepath);
			}	
		}
	}
	else {
		returnValue = printFileContent(input);
	}

	return returnValue;
}