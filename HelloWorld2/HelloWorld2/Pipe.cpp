#include "stdafx.h"
using namespace std;

Pipe::Pipe(int size) : startIndex{ 0 }, endIndex{ 0 }, entryClosed{false}, exitClosed(false), bufferSize(size) {}

Pipe::Pipe() : Pipe(DEFAULT_BUFFER_SIZE) {}

bool Pipe::PushChar(char character)
{
	unique_lock<mutex> locker(_mutex);
	cond.wait(locker, [this]() {return contentBuffer.size() < bufferSize || entryClosed; });
	if (entryClosed)
	{
		// next process will never read from this pipe
		contentBuffer.clear();
		locker.unlock();
		return false;
	}
	
	contentBuffer.push_back(character);
	locker.unlock();
	cond.notify_all();
	return true;
}

char Pipe::PopChar(bool& success)
{
	unique_lock<mutex> locker(_mutex);
	cond.wait(locker, [this]() {return contentBuffer.size() > 0 || exitClosed; });
	char value = '\0';
	if (contentBuffer.size() == 0)
	{
		success = false;
		exitClosed = true;
	}
	else 
	{
		success = true;
		value = contentBuffer.front();
		contentBuffer.pop_front();
		if (contentBuffer.size() == 0 && entryClosed)
		{
			exitClosed = true;
		}
	}
	locker.unlock();
	cond.notify_all();
	return value;
}

void Pipe::CloseEntry()
{
	unique_lock<mutex> locker(_mutex);
	entryClosed = true;
	locker.unlock();
	cond.notify_all();
}

void Pipe::CloseExit()
{
	unique_lock<mutex> locker(_mutex);
	exitClosed = true;
	locker.unlock();
	cond.notify_all();
}


