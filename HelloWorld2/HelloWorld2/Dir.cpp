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

	FileTimeToSystemTime(&time, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	string localTime = to_string(stLocal.wMonth) + "/" + to_string(stLocal.wDay) + "/" + to_string(stLocal.wYear) + "  " + to_string(stLocal.wHour) + ":" + to_string(stLocal.wMinute);


	return localTime;
}


int Dir::listDir(string path) {

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	string line;

	// Find the first file in the directory.

	hFind = kernel->OurFindFirstFile(path, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		line = "Invalid target directory.";
		output->WriteLine(line);
		return -4;
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
			line = " <DIR> ";

		}
		else
		{
			line = " <FILE>	";
		}

		filesize.LowPart = ffd.nFileSizeLow;
		filesize.HighPart = ffd.nFileSizeHigh;

		if (showAll) {
			line += to_string(ffd.dwFileAttributes) + " ";
			line += getTime(ffd.ftCreationTime) + " ";
			line += to_string(filesize.QuadPart) + " ";
		}
		line += Utils::WcharToString(ffd.cFileName);

		output->WriteLine(line);

	} while (kernel->OurFindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		output->WriteLine("FindFirstFile");
		FindClose(hFind);
	}

	return 0;

}


int Dir::RunProcess()
{
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

