#include "stdafx.h"
#include "Kernel.h"

// Mock implementation of Kernel for testing Shell::parseInput

Kernel::Kernel() : pipeCounter(0), pidCounter(0), fileSystem(nullptr) {
}

int Kernel::Execute(int parentPid, File* pathFile, string programName, string parameters, IOType inputType, string inputParam, IOType outputType, string outputParam)
{
    // Mock success
    return 123;
}

int Kernel::WaitForChildren(vector<int>& childrenPids)
{
    return 0;
}

// Implement other methods if needed by linker
// Since Shell only calls Execute and WaitForChildren (and constructor/destructor), this should be enough.
// If not, we will add more dummies.

// Note: Kernel members (mutexes, maps) are implicitly constructed/destructed.
