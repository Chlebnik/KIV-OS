#include "stdafx.h"

using namespace std;

string Dir::GetHelpContent()
{
	string help = "Displays a list of files and subdirectories in a directory.\n\nDIR[drive:][path][filename]\n\nAttribute / A will show file info.";
	return help;
}

bool Dir::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = false;
		pathIndex = -1;
	}
	else {
		for (vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (i == 0 && *it == "--help" && parameters.size() == 1) {
				showHelp = true;
				valid = true;
			}
			else if (i == 0 && *it == "/A" && parameters.size() < 3) {
				showAll = true;
				valid = true;
			}
			else if (i == 0 && parameters.size() == 1) {
				showHelp = false;
				showAll = false;
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

string Dir::getTime(const time_t* time) {
	struct tm timeinfo;
	char buffer[80];

	localtime_s(&timeinfo, time);
	
	strftime(buffer, 80, "%F %T", &timeinfo);

	string localTime = (buffer);


	return localTime;
}

int Dir::listDir(string path) {
	string line;
	int size;
	string sizeType;

	line = " TYPE	";
	if (showAll) {
		line += "CREATION_TIME       ";
		line += "FILE_SIZE ";
	}
	line += "NAME";

	output->WriteLine(line);
	int response;
	File* filepath = kernel->GetFile(path, this->GetPathFile(), response);
	vector<File*> files = filepath->GetChildren();
	for (File* file : files) {
		if (file->IsFolder()){
			line = " <DIR>  ";
			size = 0;
			sizeType = " - ";
		}else{
			line = " <FILE>	";
			size = file->GetSize();
			if (size == 0) {
				sizeType = "  ";
			}
			else if (size < 1024) {
				sizeType = "b ";
			}
			else if (size < (1024 * 1024)) {
				size = size / 1024;
				sizeType = "kb";
			}
			else if (size < (1024 * 1024 * 1024)) {
				size = size / (1024 * 1024);
				sizeType = "Mb";
			}
			else {
				size = size / (1024 * 1024 * 1024);
				sizeType = "Gb";
			}
		}
		
		if (showAll) {
			line += getTime(file->GetCreationTime()) + " ";
			if (size == 0) {
				line += "-";
			}else{
				line += size;
			}			
			line += " " + sizeType + "  ";
		}
		line += file->GetName();

		output->WriteLine(line);
	}

	return 0;
}

int Dir::RunProcess()
{
	setlocale(LC_ALL, "");
	
	int returnValue = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else if(pathIndex > -1){
		returnValue = listDir(parameters[pathIndex]);
	}
	else {
		returnValue = listDir(this->GetPathFile()->GetAbsolutePath());
	}
	return returnValue;
}

