#include "stdafx.h"
#include "Kernel.h"



string Kernel::WcharToString(wchar_t* text) {
	string new_text;
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, text, -1, ch, 260, &DefChar, NULL);
	new_text = ch;

	return new_text;
}

wchar_t* Kernel::StringToWchar(string text) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wstr, wchars_num);

	return wstr;
}


DWORD Kernel::OurGetFileAttributesA(const string& dirName_in) {
	return GetFileAttributesA(dirName_in.c_str());
}

DWORD Kernel::OurGetCurrentDirectory(_In_ DWORD BUFSIZE, _Out_ LPWSTR Buffer) {
	return GetCurrentDirectory(BUFSIZE, Buffer);
}


HANDLE Kernel::OurFindFirstFile(_In_ LPWSTR szDir, _Out_ LPWIN32_FIND_DATAW ffd) {
	return FindFirstFile(szDir, ffd);
}

DWORD Kernel::OurFindNextFile(_In_ HANDLE hFind, _Out_ LPWIN32_FIND_DATAW ffd) {
	return FindNextFile(hFind, ffd);
}
