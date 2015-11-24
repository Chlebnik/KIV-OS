#include "stdafx.h"

using namespace std;

File::File(FileAttribute fileAttribute) : fileAttribute{ fileAttribute }, content { "" }{}

int File::Open()
{
	return 0;
}

int File::Close()
{
	return 0;
}

int File::Write(string additionalContent)
{
	content.append(additionalContent);
	return 0;
}

int File::RemoveContent()
{
	content.clear();
	return 0;
}

bool File::IsRoot()
{
	return parent == NULL;
}

bool File::IsFolder()
{
	return fileAttribute == FOLDER_ATT;
}

int File::GetSize()
{
	return content.length();
}