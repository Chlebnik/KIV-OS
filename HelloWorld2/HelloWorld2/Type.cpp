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

void Type::printFileContent(AbstractInput * printInput)
{
	bool success = false;
	string line = "";
	while (printInput->HasNext()) {
		line = printInput->ReadLine(success);
		if (success) {
			output->WriteLine(line);
		}
		
	}
}

void Type::proccesFile(string filepath) {
	string basename = kernel->SplitPath(filepath, "basename");
	AbstractInput* fileFromParam = NULL;
	ifstream fileInput(filepath);
	if (!fileInput.is_open())
	{
		output->WriteLine("\nCould not open " + basename + "\n");
		return;
	}
	fileFromParam = new FileInput(fileInput, kernel);

	output->WriteLine("\n" + basename + "\n");
	printFileContent(fileFromParam);
	fileFromParam->Close();
}

int Type::listDir(string path, string regexFile) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	hFind = kernel->OurFindFirstFile(path, &ffd);

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
				proccesFile(filePath);
			}
		}

	} while (kernel->OurFindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		output->WriteLine("FindFirstFile");
		FindClose(hFind);
	}

	return 0;

}

int Type::RunProcess()
{
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
				listDir(dirPath, regexPattern);
			}
			else {
				proccesFile(filepath);
			}	
		}
	}
	else {
		printFileContent(input);
	}




	output->Close();
	input->Close();

	return 0;
}