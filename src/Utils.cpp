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

vector<string> Utils::Split(const string& str, const char& ch) {
	string next;
	vector<string> result;

	// For each character in the string
	for (string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}
