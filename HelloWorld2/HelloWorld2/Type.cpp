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
		
	}
	return returnValue;
}

int Type::proccesFile(string filepath) {
	int returnValue = 0;
	string basename = kernel->SplitPath(filepath, "basename");
	AbstractInput* fileFromParam = kernel->CreateInputClass(FILE_TYPE, filepath, this->GetPid(), GetPathFile(), returnValue);

	output->WriteLine("\n" + basename + "\n");
	returnValue = printFileContent(fileFromParam);
	fileFromParam->Close();

	return returnValue;
}

int Type::listDir(string path, string regexFile) {
	int returnValue = 0;

	File* file = kernel->GetFile(path, this->GetPathFile(), returnValue);
	if (returnValue == 0) {
		for(File* child: file->GetChildren())
		{
			string fileName = child->GetName();
			regex findFiles = regex(regexFile);
			if(regex_match(fileName, findFiles)) {
				string filePath = path + "/" + fileName;
				returnValue = proccesFile(filePath);
				if (returnValue != 0) {
					return returnValue;
				}
			}
		}
	}

	return returnValue;

}

int Type::RunProcess()
{
	returnVal = 0;
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
				returnVal = listDir(dirPath, regexPattern);
			}
			else {
				returnVal = proccesFile(filepath);
			}	
		}
	}
	else {
		returnVal = printFileContent(input);
	}
	this->Close();
	return returnVal;
}