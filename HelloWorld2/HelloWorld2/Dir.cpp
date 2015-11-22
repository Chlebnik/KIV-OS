#include "stdafx.h"

#define BUFSIZE MAX_PATH



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

void Dir::write_line(string line) {
	output->WriteLine(line);

}

string Dir::read_line() {
	string line;
	bool success = true;

	line = input->ReadLine(success);

	if (success) {
		return line;
	}
}

string Dir::getTime(FILETIME time) {
	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&time, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	string localTime = to_string(stLocal.wMonth) + "/" + to_string(stLocal.wDay) + "/" + to_string(stLocal.wYear) + "  " + to_string(stLocal.wHour) + ":" + to_string(stLocal.wMinute);


	return localTime;
}


int Dir::list_dir(string path) {

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	DWORD dwRet;
	string line;

	TCHAR Buffer[BUFSIZE];
	wchar_t* wstr;

	if (path.size() == 0) {
		dwRet = kernel->OurGetCurrentDirectory(BUFSIZE, Buffer);
		if (dwRet == 0)
		{
			line = "GetCurrentDirectory failed (";
			line += to_string(GetLastError());
			line += ")\n";
			write_line(line);
			return -1;
		}
		if (dwRet > BUFSIZE)
		{
			line = "Buffer too small; need ";
			line += to_string(dwRet);
			line += " characters\n";
			write_line(line);
			return -2;
		}
		wstr = Buffer;

	}
	else {
		wstr = kernel->StringToWchar(path);
	}

	StringCchLength(wstr, MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		write_line("\nDirectory path is too long.\n");
		return (-3);
	}

	line = "\nTarget directory is ";
	line += kernel->WcharToString(wstr) + "\n";
	write_line(line);


	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, wstr);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = kernel->OurFindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		line = "Invalid target directory.";
		write_line(line);
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

	write_line(line);
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
		line += kernel->WcharToString(ffd.cFileName);

		write_line(line);

	} while (kernel->OurFindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		write_line("FindFirstFile");
		FindClose(hFind);
	}

	return 0;

}


int Dir::RunProcess()
{
	int returnValue = 0;
	if (showHelp) {
		write_line(GetHelpContent());
	}
	else if(pathIndex > 0){
		returnValue = list_dir(parameters[pathIndex]);
	}
	else {
		returnValue = list_dir("");
	}
	return returnValue;
}

