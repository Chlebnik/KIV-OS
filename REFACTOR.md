# Refactoring Guide

This document outlines steps to modernize the KIV/OS codebase, transitioning from Java-style OOP patterns to idiomatic C++ best practices.

## Priority 1: Critical Fixes (Memory Leaks & Correctness)

### 1.1 Add Missing Destructors

**Problem**: Memory leaks in `Kernel`, `FileSystem`, and `File` classes.

**Solution**: Implement proper destructors for resource cleanup.

```cpp
// FileSystem.h - Add forward declaration
class File;

// FileSystem.cpp
FileSystem::~FileSystem() {
    for (File* drive : drives) {
        delete drive;
    }
    drives.clear();
}

// File.h - Add destructor
File::~File() {
    for (File* child : children) {
        delete child;
    }
    children.clear();
}

// Kernel.h - Add forward declarations
class Pipe;

// Kernel.cpp
Kernel::~Kernel() {
    delete fileSystem;
    
    for (auto& pair : pipeMap) {
        delete pair.second;
    }
    pipeMap.clear();
    
    for (auto& pair : processMap) {
        delete pair.second;
    }
    processMap.clear();
}
```

### 1.2 Fix EOF Handling

**Problem**: `File::ReadChar` returns `char` but uses `EOF = 26`, preventing byte 26 from being read.

**Solution**: Change return type to `int`.

```cpp
// File.h
int ReadChar(FileInput* reader, bool& success);

// File.cpp
int File::ReadChar(FileInput* reader, bool& success) {
    // ... existing logic ...
    if (/* end of file */) {
        success = false;
        return EOF;  // Use -1 from <cstdio>
    }
    // ...
}
```

Remove the `#define EOF 26` from `Kernel.h`.

### 1.3 Add Virtual Destructor to AbstractProcess

**Problem**: Base class with virtual functions but non-virtual destructor.

```cpp
// AbstractProcess.h
virtual ~AbstractProcess();
```

---

## Priority 2: Thread Safety

### 2.1 Fix processMap Race Condition

**Problem**: `processMap` is accessed without consistent locking in `Execute` and `CreatePipe`.

**Solution**: Protect all accesses with `mutexProcess`.

```cpp
// Kernel.cpp - Execute
int Kernel::Execute(...) {
    // ...
    {
        lock_guard<mutex> lock(mutexProcess);
        processMap[pid] = process;
    }
    // ...
}

// Kernel.cpp - CreatePipe
int Kernel::CreatePipe(...) {
    // ...
    lock_guard<mutex> lock(mutexProcess);
    // ... access processMap if needed ...
}
```

### 2.2 Fix Thread Overwrite in AbstractProcess::Run

**Problem**: Calling `Run()` multiple times leaks the previous thread.

```cpp
// AbstractProcess.cpp
int AbstractProcess::Run() {
    if (t && t->joinable()) {
        return returnVal;  // Already running
    }
    t = new thread(&AbstractProcess::RunProcess, this);
    return 0;
}
```

Or better - use `std::thread` directly (not pointer):

```cpp
// AbstractProcess.h
private:
    thread t;  // Not thread*
```

---

## Priority 3: Modern C++ Smart Pointers

### 3.1 Replace Raw Pointers with Smart Pointers

Replace owning raw pointers with `std::unique_ptr` or `std::shared_ptr`.

```cpp
// Kernel.h - Before
FileSystem* fileSystem;
map<int, Pipe*> pipeMap;
map<int, AbstractProcess*> processMap;

// Kernel.h - After
std::unique_ptr<FileSystem> fileSystem;
std::map<int, std::unique_ptr<Pipe>> pipeMap;
std::map<int, std::unique_ptr<AbstractProcess>> processMap;
```

### 3.2 Update Factory Methods

```cpp
// Kernel.cpp - Before
AbstractProcess* Kernel::CreateProcessClass(string programName, int parentPid) {
    // ...
    return new Shell(pid, parentPid, this);
}

// Kernel.cpp - After
std::unique_ptr<AbstractProcess> Kernel::CreateProcessClass(string programName, int parentPid) {
    // ...
    return std::make_unique<Shell>(pid, parentPid, this);
}
```

### 3.3 Update Access Patterns

```cpp
// Kernel.cpp - Before
Pipe* pipe = pipeMap[pipeIndex];
if (pipe) { ... }

// Kernel.cpp - After
auto it = pipeMap.find(pipeIndex);
if (it != pipeMap.end()) {
    Pipe* pipe = it->second.get();
    // ...
}
```

---

## Priority 4: Code Style & Best Practices

### 4.1 Remove `using namespace std;` from Headers

```cpp
// Kernel.h - Before
using namespace std;

// Kernel.h - After
// Remove the directive entirely
// Use std:: prefix: std::string, std::map, etc.
```

### 4.2 Use Enums Instead of Magic Numbers

```cpp
// ErrorCodes.h - Create proper enum
enum class ErrorCode {
    Success = 0,
    FileNotFound = 2,
    AccessDenied = 5,
    InvalidPath = 8,
    // ...
};

// Usage
ErrorCode result = ErrorCode::Success;
```

### 4.3 Pass Strings by const Reference

```cpp
// FileSystem.h - Before
File* GetFile(string path, File* sourceFile, int& response);

// FileSystem.h - After
File* GetFile(const std::string& path, File* sourceFile, int& response);
```

### 4.4 Use constexpr for Constants

```cpp
// Pipe.h - Before
static const int DEFAULT_BUFFER_SIZE = 64 * 1024;

// Pipe.h - After
static constexpr size_t DEFAULT_BUFFER_SIZE = 64 * 1024;
```

### 4.5 Return References Where Appropriate

```cpp
// File.h - Before
vector<File*> GetChildren();

// File.h - After
const std::vector<File*>& GetChildren() const;
```

### 4.6 Use `std::optional` for Nullable Returns

```cpp
// FileSystem.h - Before
File* GetFile(const std::string& path, File* sourceFile, int& response);

// FileSystem.h - After
std::optional<File*> GetFile(const std::string& path, File* sourceFile);
```

---

## Priority 5: Modern C++ Patterns

### 5.1 Use `std::thread` by Value

```cpp
// AbstractProcess.h - Before
thread* t;

// AbstractProcess.h - After
std::thread t;
```

### 5.2 Use `std::atomic` Directly

```cpp
// Kernel.h - Before
atomic_int pidCounter;

// Kernel.h - After
std::atomic<int> pidCounter;
```

### 5.3 Replace `vector::erase` with Swap-and-Pop

```cpp
// File.cpp - Before
vector<File*>::iterator it = std::find(children.begin(), children.end(), f);
if (it != children.end()) {
    children.erase(it);
}

// File.cpp - After
auto it = std::find(children.begin(), children.end(), f);
if (it != children.end()) {
    std::swap(*it, children.back());
    children.pop_back();
}
```

### 5.4 Use Range-Based for Loops

```cpp
// FileSystem.cpp - Before
for (vector<File*>::iterator iterator = drives.begin(); iterator != drives.end(); ++iterator)

// FileSystem.cpp - After
for (File* drive : drives)
```

---

## Priority 6: Architectural Improvements

### 6.1 Consider `std::variant` for Type Erasure

Instead of `enum + pointer`, use `std::variant`:

```cpp
// Before
enum IOType { PIPE_SINGLE_TYPE, PIPE_BOTH_TYPE, FILE_TYPE, STANDARD_TYPE };
AbstractInput* CreateInputClass(IOType type, ...);

// After
using InputSource = std::variant<
    std::monostate,  // STANDARD_TYPE
    int,             // PIPE index
    std::string      // file path
>;
```

### 6.2 Use `std::function` for Callbacks

Replace function pointers with `std::function` where appropriate.

### 6.3 Consider RAII Lock Guards

Replace manual lock/unlock with RAII:

```cpp
// Before
mutexProcess.lock();
processMap[pid] = process;
mutexProcess.unlock();

// After
{
    std::lock_guard<std::mutex> lock(mutexProcess);
    processMap[pid] = std::move(process);
}
```

---

## Refactoring Order

1. **Phase 1**: Fix critical bugs (destructors, EOF, virtual destructor)
2. **Phase 2**: Fix thread safety issues
3. **Phase 3**: Replace raw pointers with smart pointers
4. **Phase 4**: Apply code style improvements
5. **Phase 5**: Modernize to C++11/14/17 patterns
6. **Phase 6**: Consider architectural changes

---

## Testing After Each Phase

- Run test suite: `./tests/run_tests.sh`
- Check for memory leaks with Valgrind: `valgrind --leak-check=full ./kivos`
- Verify thread safety with ThreadSanitizer: `g++ -fsanitize=thread ...`

---

## References

- C++ Core Guidelines: https://isocpp.github.io/CppCoreGuidelines/
- `ISSUES.md`: Original issue list
- `ARCHITECTURE.md`: Current architecture overview
