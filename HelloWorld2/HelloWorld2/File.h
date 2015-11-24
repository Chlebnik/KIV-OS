#pragma once
#include "stdafx.h"

using namespace std;

enum FileAttribute{FOLDER_ATT, FILE_ATT};

class File
{
private:
	int size;
	File* parent;
	string content;
	vector<File*> children;
	FileAttribute fileAttribute;
	string name;

public:
	File(FileAttribute fileAttribute);
	int Open();
	int Close();
	int Write(string additionalContet);
	int RemoveContent();
	bool IsRoot();
	bool IsFolder();
	int GetSize();
};
