#include "stdafx.h"

using namespace std;

File::File(FileAttribute fileAttribute) : fileAttribute{ fileAttribute }, content { "" }{}

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
	// TODO
	//iterator = content.begin();
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
	if (readerMap.find(reader) != readerMap.end())
	{
		readerMap.erase(reader);
	}
	locker.unlock();
	return 0;
}

int File::Write(FileOutput* writer, string additionalContent)
{
	unique_lock<mutex> locker(mutexIO);
	// TODO
	locker.unlock();
	content.append(additionalContent);
	return 0;
}

bool File::VerifyWriter(FileOutput* writer)
{
	return find(writers.begin(), writers.end(), writer) != writers.end();
}

int File::RemoveContent(FileOutput* writer)
{
	int retVal = 0;
	unique_lock<mutex> locker(mutexIO);
	if (VerifyWriter(writer))
	{
		content.clear();
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

int File::GetSize()
{
	return content.length();
}

char File::ReadChar(FileInput* reader, bool& success)
{
	unique_lock<mutex> locker(mutexIO);
	locker.unlock();
}