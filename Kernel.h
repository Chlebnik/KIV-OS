#ifndef KERNEL_H
#define KERNEL_H

#include <iostream>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <tchar.h>

using namespace std;

class Kernel
{
public:
	Kernel() {};
	~Kernel() {};
	string WcharToString(wchar_t* text);
	wchar_t* StringToWchar(string text);
	DWORD OurGetFileAttributesA(const string& dirName_in);
	DWORD OurGetCurrentDirectory(_In_ DWORD BUFSIZE, _Out_ LPWSTR Buffer);
	HANDLE OurFindFirstFile(_In_ LPWSTR szDir, _Out_ LPWIN32_FIND_DATAW ffd);
	DWORD OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd);
};

#endif KERNEL_H

