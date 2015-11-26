#pragma once
#include "stdafx.h"

using namespace std;

class FileSystem
{
private:
	vector<File*> drives;
	bool ValidatePath(string path);

public:
	File* CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response);
	File* CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response);
	File* GetFile(string path, int& response);
	int RemoveFile(string path);
	int RemoveFile(File* file);
};
