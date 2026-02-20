// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#include <time.h> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <random>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <map>
#include <regex>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#else
#include <climits>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <memory>

#define BOOL int
#define TRUE 1
#define FALSE 0
#define boolean bool
#define TCHAR char
#define _T(x) x
#define _In_
#define _MAX_DRIVE 3
#define _MAX_DIR 256
#define _MAX_FNAME 256
#define _MAX_EXT 256
#define MAX_PATH 260
#define FILE_SEPARATOR '/'

typedef int* PBOOL;
typedef void* HANDLE;

// Stub for QueryMemoryResourceNotification related
#define LowMemoryResourceNotification 0
inline HANDLE CreateMemoryResourceNotification(int) { return (HANDLE)0; }
inline BOOL QueryMemoryResourceNotification(HANDLE, PBOOL) { return FALSE; }

#endif

#include "targetver.h"

#include "ErrorCodes.h"
#include "Utils.h"
#include "FirstLineFromFile.h"

#include "Pipe.h"
#include "File.h"
#include "FileSystem.h"
#include "Kernel.h"

#include "AbstractIO.h"

#include "AbstractInput.h"
#include "StandardInput.h"
#include "FileInput.h"
#include "PipeInput.h"

#include "AbstractOutput.h"
#include "StandardOutput.h"
#include "FileOutput.h"
#include "PipeOutput.h"

#include "AbstractProcess.h"
#include "Shell.h"
#include "Sort.h"
#include "Rand.h"
#include "ChangeDirectory.h"
#include "Dir.h"
#include "Echo.h"
#include "WordCount.h"
#include "Type.h"
#include "MakeDirectory.h"
#include "Freq.h"
#include "Remove.h"

#define SHELLS_PID 1
// TODO: reference additional headers your program requires here
