#pragma once
#include "stdafx.h"

using namespace std;

class Utils
{
public:
	static string WcharToString(wchar_t* text);
	static wchar_t* StringToWchar(string text);
};

