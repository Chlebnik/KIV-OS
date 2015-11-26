#include "stdafx.h"

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response)
{
	return NULL;
}

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response)
{
	return NULL;
}
File* FileSystem::GetFile(string path, int& response)
{
	return NULL;
}
int FileSystem::RemoveFile(string path)
{
	return 0;
}
int FileSystem::RemoveFile(File* file)
{
	return 0;
}