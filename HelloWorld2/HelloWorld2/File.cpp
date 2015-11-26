#include "stdafx.h"

using namespace std;

File::File(FileAttribute fileAttribute, File* parent) : fileAttribute{ fileAttribute }, parent{parent}, content { "" } {}

int File::OpenReader(FileInput* reader)
{
	unique_lock<mutex> locker(mutexIO);
	readerMap[reader] = 0;
	locker.unlock();
	return 0;
}

int File::OpenWriter(FileOutput* writer)
{
	unique_lock<mutex> locker(mutexIO);
	if (!VerifyWriter(writer))
	{
		writers.push_back(writer);
	}
	locker.unlock();
	return 0;
}

int File::CloseWriter(FileOutput* writer)
{
	unique_lock<mutex> locker(mutexIO);
	writers.erase(remove(writers.begin(), writers.end(), writer), writers.end());
	locker.unlock();
	return 0;
}

int File::CloseReader(FileInput* reader)
{
	unique_lock<mutex> locker(mutexIO);
	if (VerifyReader(reader))
	{
		readerMap.erase(reader);
	}
	locker.unlock();
	return 0;
}

int File::Write(FileOutput* writer, string additionalContent)
{
	unique_lock<mutex> locker(mutexIO);
	content.append(additionalContent);
	locker.unlock();
	return 0;
}

bool File::VerifyWriter(FileOutput* writer)
{
	return find(writers.begin(), writers.end(), writer) != writers.end();
}

bool File::VerifyReader(FileInput* reader)
{
	return readerMap.find(reader) != readerMap.end();
}


int File::RemoveContent(FileOutput* writer)
{
	int retVal = 0;
	unique_lock<mutex> locker(mutexIO);
	if (VerifyWriter(writer))
	{
		content.clear();
		for (auto iterator = readerMap.begin(); iterator != readerMap.end(); ++iterator)
		{
			iterator->second = 0;
		}
	}
	else
	{
		retVal = 1; //TODO ERROR adding
	}
	locker.unlock();

	return retVal;
}

bool File::IsRoot()
{
	return parent == NULL;
}

bool File::IsFolder()
{
	return fileAttribute == FOLDER_ATT;
}

size_t File::GetSize()
{
	return content.length();
}

char File::ReadChar(FileInput* reader, bool& success)
{
	char value = EOF;
	unique_lock<mutex> locker(mutexIO);
	if (VerifyReader(reader))
	{
		size_t offset = readerMap[reader];
		if (offset < content.length())
		{
			string::iterator iterator = content.begin() + offset;
			value = *iterator;
			readerMap[reader] = offset + 1;
			success = true;
		}
		else
		{
			success = false;
		}
	}
	else
	{
		success = false;
	}
	locker.unlock();
	return value;
}

string File::GetAbsolutePath()
{
	if (parent == NULL)
	{
		return name + ":";
	}
	else
	{
		return parent->GetAbsolutePath() + FILE_SEPARATOR + name;
	}
}

string File::GetName()
{
	return name;
}

File* File::GetParent()
{
	return parent;
}

int File::AddChild(File* f)
{
	if (!IsFolder())
	{
		//TODO error - file is not folder
		return 3;
	}
	int retVal = 0;
	unique_lock<mutex> locker(mutexIO);
	children.push_back(f);
	locker.unlock();


	return retVal;
}

vector<File*> File::GetChildren()
{
	vector<File*> retVal;
	unique_lock<mutex> locker(mutexIO);
	retVal = children;
	locker.unlock();
	return retVal;
}