#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h> 
#include <strsafe.h>
#include "Kernel.h"

#pragma comment(lib, "User32.lib")

#define BUFSIZE MAX_PATH

using namespace std;

Kernel kernel;

void write_line(string line) {

	cout << line << endl;
}

string read_line() {
	string line;
	getline(cin, line);

	return line;
}

string getTime(FILETIME time) {
	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&time, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	string localTime = to_string(stLocal.wMonth) +"/"+ to_string(stLocal.wDay) +"/"+ to_string(stLocal.wYear) +"  " + to_string(stLocal.wHour) +":" + to_string(stLocal.wMinute);
	

	return localTime;
}

int list_dir(string path, bool attr) {
	
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
		dwRet = kernel.OurGetCurrentDirectory(BUFSIZE, Buffer);
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

	}else{
		wstr = kernel.StringToWchar(path);
	}

	StringCchLength(wstr, MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		write_line("\nDirectory path is too long.\n");
		return (-3);
	}

	line = "\nTarget directory is ";
	line += kernel.WcharToString(wstr)+"\n";
	write_line(line);
	

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, wstr);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = kernel.OurFindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		line = "Invalid target directory.";
		write_line(line);
		return -4;
	}

	// List all the files in the directory with some info about them.
	line = " TYPE	";
	if(attr){
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

		if (attr) {
			line += to_string(ffd.dwFileAttributes) + " ";
			line += getTime(ffd.ftCreationTime) + " ";
			line += to_string(filesize.QuadPart) + " ";
		}
		line += kernel.WcharToString(ffd.cFileName);

		write_line(line);

	} while (kernel.OurFindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		write_line("FindFirstFile");
		FindClose(hFind);
	}

	return 0;

}
//Parameters validation
int checkParams(int argc, char* argv[]) {
	int valid = 1;
	bool atr = false;
	if (argc == 1) {
		list_dir("", atr);
	}
	for (int i = 1; i < argc; i++) {
		string checkValue = argv[i];
		if (checkValue == "--help") {
			write_line("Displays a list of files and subdirectories in a directory.\n\nDIR [drive:][path][filename]\n\nAttribute /A will show file info.");
			exit(EXIT_SUCCESS);
		}
		else if (checkValue == "/A") {
			atr = true;
			if (argc == 2) {
				valid = list_dir("", atr);
			}
		}
		else {
			valid = list_dir(checkValue, atr);
			if (valid != 0) {
				return valid;
			}
		}
	}

	return valid;
}

int main(int argc, char* argv[])
{
	kernel = Kernel();
	return checkParams(argc, argv);


}

