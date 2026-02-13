# Issues Report

This report identifies leaks, errors, best practice violations, and performance improvement opportunities in the KIV/OS codebase.

## Memory Leaks

### 1. `Kernel` Class Leaks Resources
- **Location:** `src/Kernel.h`, `src/Kernel.cpp`
- **Description:** The `Kernel` class manages several dynamic resources (`fileSystem`, `pipeMap`, `processMap`) but lacks a destructor to clean them up.
- **Impact:** When a `Kernel` instance is destroyed (e.g., at the end of `main`), all these resources are leaked.
- **Suggestion:** Implement `~Kernel()` to delete `fileSystem`, all `Pipe*` in `pipeMap`, and all `AbstractProcess*` in `processMap`.

### 2. `FileSystem` Class Leaks Drives
- **Location:** `src/FileSystem.h`, `src/FileSystem.cpp`
- **Description:** The `FileSystem` class stores `File*` objects in `drives` vector but lacks a destructor.
- **Impact:** All drives and their contents are leaked when `FileSystem` is destroyed.
- **Suggestion:** Implement `~FileSystem()` to delete all `File*` in `drives`.

### 3. `File` Class Leaks Children
- **Location:** `src/File.h`, `src/File.cpp`
- **Description:** The `File` class stores `File*` children in `children` vector but lacks a destructor.
- **Impact:** When a directory is deleted, its children are leaked (unless recursively deleted manually, which is not done).
- **Suggestion:** Implement `~File()` to delete all `File*` in `children`.

### 4. `FileSystem::RemoveFile` Leaks Children
- **Location:** `src/FileSystem.cpp` (lines ~138-142)
- **Description:** When removing a directory, `delete file` is called. Since `File` has no destructor, its children are not deleted.
- **Impact:** Significant memory leak when removing non-empty directories.
- **Suggestion:** Rely on the `~File()` destructor (once implemented) to handle recursive deletion.

## Concurrency Issues

### 1. `processMap` Race Condition in `Kernel`
- **Location:** `src/Kernel.cpp`
- **Description:** `processMap` is accessed in `Execute` (write), `WaitForChildren` (read/write), and `CreatePipe` (read/write). While `WaitForChildren` uses `mutexProcess`, `Execute` and `CreatePipe` do not consistently protect `processMap`. Specifically, `Execute` writes to it without locking, which can race with `WaitForChildren` or other `Execute` calls if multiple threads spawn processes.
- **Impact:** Undefined behavior, potential crash or corruption of process tracking.
- **Suggestion:** Ensure all accesses to `processMap` are protected by `mutexProcess`.

### 2. `AbstractProcess::Run` Thread Overwrite
- **Location:** `src/AbstractProcess.cpp`
- **Description:** `Run()` creates a new `thread` and assigns it to `t`. If called multiple times without `Join()`, the old `thread*` is overwritten and leaked.
- **Impact:** Memory leak of `thread` object, potential `std::terminate` if the old thread was joinable.
- **Suggestion:** Check if `t` is already set/running before creating a new one, or ensure correct lifecycle management.

## Coding Errors

### 1. `File::ReadChar` Returns `EOF` as `char`
- **Location:** `src/File.cpp`, `src/Kernel.h`
- **Description:** `ReadChar` returns `char`. `EOF` is defined as `26` (ASCII SUB) in `Kernel.h`. This prevents the byte `26` from being read as valid data. Standard `fgetc` returns `int` to distinguish `EOF` (-1 usually) from any valid `unsigned char` value.
- **Impact:** Binary files containing byte 0x1A (26) will be truncated or read incorrectly.
- **Suggestion:** Change return type to `int` and use a value outside `char` range (e.g., -1) for EOF.

### 2. `FileSystem::ValidateFileName` Casting
- **Location:** `src/FileSystem.cpp`
- **Description:** `(int)path.length()` casts `size_t` to `int`. If a path is extremely long (> 2GB), this wraps around to negative, causing logic errors.
- **Impact:** Unlikely in this simulation, but bad practice.
- **Suggestion:** Use `size_t` for length and loop indices.

### 3. Missing Virtual Destructor in `AbstractProcess`
- **Location:** `src/AbstractProcess.h`
- **Description:** `AbstractProcess` has virtual functions but a non-virtual destructor.
- **Impact:** If a derived class (e.g., `Shell`) has its own resources (it doesn't currently, but might in future), deleting it via `AbstractProcess*` results in undefined behavior.
- **Suggestion:** Declare `virtual ~AbstractProcess();`.

## Best Practice Violations

### 1. `using namespace std;` in Header Files
- **Location:** `src/Kernel.h`, `src/FileSystem.h`, `src/AbstractProcess.h`, `src/File.h`
- **Description:** Importing the entire `std` namespace in headers pollutes the global namespace for any file including them.
- **Suggestion:** Remove `using namespace std;` from headers. Use `std::` prefix or putting `using` in .cpp files only.

### 2. `#define EOF 26`
- **Location:** `src/Kernel.h`
- **Description:** `EOF` is a standard macro (usually -1). Redefining it can cause conflicts with standard libraries.
- **Suggestion:** Use a different name (e.g., `K_EOF`) or a typed constant.

### 3. Raw Pointers for Ownership
- **Location:** Everywhere
- **Description:** The codebase heavily uses raw pointers for owning resources (`File*`, `AbstractProcess*`, etc.), leading to the memory leaks identified above.
- **Suggestion:** Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) to automate memory management and ownership.

### 4. Inconsistent Error Handling
- **Location:** `src/Kernel.cpp`, `src/FileSystem.cpp`
- **Description:** Functions return `int` error codes but also use `int& response` reference parameters. Magic numbers (e.g., 5, 8, 27) are used instead of named constants.
- **Suggestion:** Standardize on return values or exceptions. Use `enum` or defined constants for error codes.

## Performance Improvements

### 1. Pass Strings by Reference
- **Location:** Everywhere (e.g., `FileSystem::GetFile`, `Kernel::Write`)
- **Description:** Strings are frequently passed by value, causing unnecessary copies.
- **Suggestion:** Pass strings by const reference (`const std::string&`) where possible.

### 2. Return Vectors by Value
- **Location:** `File::GetChildren`
- **Description:** Returns a `vector<File*>` by value, which copies the vector.
- **Suggestion:** While modern C++ has RVO, in some cases creating a copy might still be expensive if the vector is large. Consider returning `const vector<File*>&` if thread safety allows (it might not here due to mutex), or moving.

### 3. `vector::erase` in Loop
- **Location:** `File::RemoveChild`
- **Description:** Erasing from a vector shifts all subsequent elements. If done frequently on large vectors, this is O(N).
- **Suggestion:** If order doesn't matter, swap with the last element and pop back (O(1)).
