#include "stdafx.h"
using namespace std;

int StandardOutput::Close()
{
	return 0;
}

int StandardOutput::Write(string s)
{
	return GetKernel()->WriteOnMonitor(s);
}

int StandardOutput::WriteLine(string s)
{
	return GetKernel()->WriteLineOnMonitor(s);
}
