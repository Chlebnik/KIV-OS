#pragma once
#include "stdafx.h"

using namespace std;

enum FileAttribute{FOLDER_ATT, FILE_ATT};
class FileInput;
class FileOutput;
class File
{
private:
	int size;
	File* parent;
	string content;
	vector<File*> children;
	FileAttribute fileAttribute;
	string name;
	map<FileInput*, int> readerMap;
	vector<FileOutput*> writers;
	mutex mutexIO;

	bool VerifyWriter(FileOutput* writer);

public:
	File(FileAttribute fileAttribute);
	int OpenReader(FileInput* reader);
	int OpenWriter(FileOutput* writer);
	int CloseReader(FileInput* reader);
	int CloseWriter(FileOutput* writer);
	int Write(FileOutput* writer, string additionalContet);
	char ReadChar(FileInput* reader, bool& success);
	int RemoveContent(FileOutput* writer);
	bool IsRoot();
	bool IsFolder();
	bool IsProcessed();
	int GetSize();
};
