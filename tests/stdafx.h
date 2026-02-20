#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <map>
#include <deque>

using namespace std;

#define SHELLS_PID 1
#define ERROR_INVALID_FILE_ATTRIBUTES 123
#define ERROR_FILE_NOT_FOUND 2

class AbstractInput;
class AbstractOutput;
class File;
class Kernel;
class FileSystem;

class File {
public:
    virtual ~File() {}
    bool IsFolder() { return true; }
};

class AbstractInput {
public:
    virtual ~AbstractInput() {}
    virtual void Close() {}
};

class AbstractOutput {
public:
    virtual ~AbstractOutput() {}
    virtual void Close() {}
    virtual void WriteLine(string s) {}
};

class Kernel {
public:
    virtual ~Kernel() {}
    File* GetFile(string path, File* source, int& response) {
         response = 0;
         return new File();
    }
    bool UpdateProcessPathFile(int pid, File* file) { return true; }
};

// Include headers that we want to mock or use from source
// We include Utils.h first as it is simple
#include "Utils.h"

// We include AbstractProcess.h as ChangeDirectory inherits from it
#include "AbstractProcess.h"

// We include ChangeDirectory.h because ChangeDirectory.cpp expects it from stdafx.h
#include "ChangeDirectory.h"
