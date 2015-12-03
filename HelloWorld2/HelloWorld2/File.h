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
	map<FileInput*, size_t> readerMap;
	vector<FileOutput*> writers;
	mutex mutexIO;

	bool VerifyWriter(FileOutput* writer);
	bool VerifyReader(FileInput* reader);

public:
	File(string name, FileAttribute fileAttribute, File* parent);
	int OpenReader(FileInput* reader);
	int OpenWriter(FileOutput* writer);
	int CloseReader(FileInput* reader);
	int CloseWriter(FileOutput* writer);
	int Write(FileOutput* writer, string additionalContet);
	char ReadChar(FileInput* reader, bool& success);
	int RemoveContent(FileOutput* writer);
	bool IsRoot();
	bool IsFolder();
	string GetAbsolutePath();
	string GetName();
	File* GetParent();
	int AddChild(File* f);
	int RemoveChild(File* f);
	vector<File*> GetChildren();
	bool IsProcessed();
	size_t GetSize();
	bool IsDeletable();
};
