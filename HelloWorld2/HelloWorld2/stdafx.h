// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <atomic>
#include <thread>
#include <mutex>
#include <deque>
#include <map>
#include <unordered_map>
#include <windows.h>
#include <strsafe.h>

#include "targetver.h"

#include "ErrorCodes.h"
#include "Utils.h"

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

// TODO: reference additional headers your program requires here
