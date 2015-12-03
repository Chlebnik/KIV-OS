#include "stdafx.h"



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
		for (std::vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
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
				valid = true;
			}
			i++;
		}
	}

	return valid;
}

string Dir::getTime(FILETIME time) {
	SYSTEMTIME stUTC, stLocal;
	char buffer[11];
	int n;

	FileTimeToSystemTime(&time, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	
	n = sprintf_s(buffer, "%02d/%02d/%04d", stLocal.wMonth, stLocal.wDay, stLocal.wYear);
	string localTime = (buffer);


	return localTime;
}


int Dir::listDir(string path) {

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	int size;
	string sizeType = "";
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	string line;

	// Find the first file in the directory.

	hFind = kernel->OurFindFirstFile(path, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		line = "Invalid target directory.";
		output->WriteLine(line);
		return ERROR_INVALID_HANDLE_VALUE;
	}

	// List all the files in the directory with some info about them.
	line = " TYPE	";
	if (showAll) {
		line += "ATTRS ";
		line += "CREATION_TIME ";
		line += "FILE_SIZE ";
	}
	line += "NAME";

	output->WriteLine(line);
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			line = " <DIR>  ";

		}
		else
		{
			line = " <FILE>	";
		}

		filesize.LowPart = ffd.nFileSizeLow;
		filesize.HighPart = ffd.nFileSizeHigh;
		if (filesize.QuadPart == 0) {
			size = filesize.QuadPart;
			sizeType = "  ";
		} else if (filesize.QuadPart < 1024) {
			size = filesize.QuadPart;
			sizeType = "b ";
		}else if (filesize.QuadPart < (1024 * 1024)) {
			size = filesize.QuadPart / 1024;
			sizeType = "kb";
		}
		else if (filesize.QuadPart < (1024 * 1024 * 1024)) {
			size = filesize.QuadPart / (1024 * 1024);
			sizeType = "Mb";
		}
		else {
			size = filesize.QuadPart / (1024 * 1024 * 1024);
			sizeType = "Gb";
		}
		

		if (showAll) {
			line += to_string(ffd.dwFileAttributes) + "    ";
			line += getTime(ffd.ftCreationTime) + "    ";		
			char buffer[50];			
			sprintf_s(buffer, "%5d", size);
			line += buffer;
			line += " " + sizeType + "  ";
		}
		line += Utils::WcharToString(ffd.cFileName);
		
		output->WriteLine(line);

	} while (kernel->OurFindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		FindClose(hFind);
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
	else if(pathIndex > 0){
		returnValue = listDir(parameters[pathIndex]);
	}
	else {
		returnValue = listDir("");
	}
	return returnValue;
}

