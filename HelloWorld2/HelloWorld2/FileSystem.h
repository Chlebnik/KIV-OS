#pragma once
#include "stdafx.h"

using namespace std;

class FileSystem
{
private:
	vector<File*> drives;

public:
	File* CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response);
};
