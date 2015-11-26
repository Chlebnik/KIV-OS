#pragma once
#include "stdafx.h"

#define FILE_SEPARATOR '/'
#define CURRENT_FOLDER "."
#define PARENT_FOLDER ".."
#define DRIVE_SUFFIX ':'

using namespace std;

class FileSystem
{
private:
	vector<File*> drives;
	bool ValidatePath(string path);
	bool ValidateFileName(string fileName);

public:
	File* CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response);
	File* CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response);
	File* GetFile(string path, File* sourceFile, int& response);
	int RemoveFile(string path);
	int RemoveFile(File* file);
};
