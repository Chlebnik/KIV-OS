#include "stdafx.h"


string Utils::WcharToString(wchar_t* text) {
	string new_text;
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, text, -1, ch, 260, &DefChar, NULL);
	new_text = ch;

	return new_text;
}

wchar_t* Utils::StringToWchar(string text) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wstr, wchars_num);

	return wstr;
}
