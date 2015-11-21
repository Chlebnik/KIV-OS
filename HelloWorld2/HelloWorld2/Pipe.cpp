#include "stdafx.h"
using namespace std;

Pipe::Pipe(int size) : startIndex{ 0 }, endIndex{ 0 }, inputClosed{false}, outputClosed(false), bufferSize(size) {}

Pipe::Pipe() : Pipe(DEFAULT_BUFFER_SIZE) {}

bool Pipe::PushChar(char character)
{
	unique_lock<mutex> locker(_mutex);
	cond.wait(locker, [this]() {return contentBuffer.size() < bufferSize || inputClosed; });
	if (inputClosed)
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
	cond.wait(locker, [this]() {return contentBuffer.size() > 0 || outputClosed; });
	char value = '\0';
	if (contentBuffer.size() == 0)
	{
		cout << "success falase";
		success = false;
		inputClosed = true;
	}
	else 
	{
		success = true;
		value = contentBuffer.front();
		contentBuffer.pop_front();
	}
	locker.unlock();
	cond.notify_all();
	return value;
}

void Pipe::CloseInputSide()
{
	unique_lock<mutex> locker(_mutex);
	inputClosed = true;
	locker.unlock();
	cond.notify_all();
}

void Pipe::CloseOutputSide()
{
	cout << "close output";
	unique_lock<mutex> locker(_mutex);
	outputClosed = true;
	locker.unlock();
	cond.notify_all();
}


