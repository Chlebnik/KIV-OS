Git repozitory for KIV/OS

string wcharToString(wchar_t* text) {
	string new_text;
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, text, -1, ch, 260, &DefChar, NULL);
	new_text = ch;

	return new_text;
}

wchar_t* stringToWchar(string text) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wstr, wchars_num);

	return wstr;
}


DWORD GetFileAttributesA(const string& dirName_in){
	return GetFileAttributesA(dirName_in.c_str());
}

DWORD GetCurrentDirectory(BUFSIZE, TCHAR& Buffer){
	return GetCurrentDirectory(BUFSIZE, Buffer);
}

	
HANDLE FindFirstFile(TCHAR szDir,WIN32_FIND_DATA& ffd){
	return FindFirstFile(szDir, &ffd);
}

DWORD FindNextFile(HANDLE hFind,WIN32_FIND_DATA& ffd){
	return FindNextFile(hFind, &ffd);
}