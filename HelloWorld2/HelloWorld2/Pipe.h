#pragma once
#include "stdafx.h"
using namespace std;

class Pipe
{
private:
	static const int DEFAULT_BUFFER_SIZE = 64 * 1024;
	int bufferSize;
	deque<char> contentBuffer;
	atomic_int startIndex;
	atomic_int endIndex;
	atomic_bool inputClosed;
	atomic_bool outputClosed;
	atomic_bool readerSleeping;
	thread reader;
	mutex _mutex;
	condition_variable cond;

public:
	Pipe();
	Pipe(int size);
	~Pipe();
	bool PushChar(char c);
	char PopChar(bool& success);
	void CloseInputSide();
	void CloseOutputSide();
};
