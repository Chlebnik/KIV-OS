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

string Type::getBasename(string filepath) {
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	
	const char* path = filepath.c_str();

	_splitpath_s(path, drive, dir, fname, ext);

	string filename(fname);
	string fileext(ext);

	return filename + fileext;
}

void Type::proccesFile(string filepath) {
	AbstractInput* fileFromParam = NULL;
	ifstream fileInput(filepath);
	if (!fileInput.is_open())
	{
		output->WriteLine("\nCould not open " + getBasename(filepath) + "\n");
		return;
	}
	fileFromParam = new FileInput(fileInput, kernel);

	output->WriteLine("\n" + getBasename(filepath) + "\n");
	printFileContent(fileFromParam);
	fileFromParam->Close();
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
				//TODO
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