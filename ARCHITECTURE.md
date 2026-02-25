# Architecture

This document describes the architecture of KIV/OS, a minimalistic OS simulation written in C++ by developers with a Java background.

## Design Philosophy

The codebase reflects its Java origins through heavy use of object-oriented patterns that are common in Java but atypical for C++:

- **Extensive inheritance hierarchies**
- **Abstract base classes for all major components**
- **Factory pattern for object creation**
- **Single-root class hierarchy for I/O**
- **Raw pointer ownership throughout**

## Core Components

### Kernel (`Kernel.h`, `Kernel.cpp`)

The central hub of the OS simulation, similar to a Java singleton managing all system resources:

- **FileSystem**: Manages the virtual RAM disk
- **Process Management**: Tracks running processes via `processMap`
- **Pipe Management**: Handles inter-process communication via `pipeMap`
- **I/O Factory**: Creates input/output streams based on type

```cpp
class Kernel {
    FileSystem* fileSystem;
    map<int, Pipe*> pipeMap;
    map<int, AbstractProcess*> processMap;
};
```

### File System (`FileSystem.h`, `FileSystem.cpp`)

Implements a virtual in-memory file system with:

- Drive-based structure (e.g., `C:`, `D:`)
- Directory hierarchy with parent-child relationships
- File validation (naming rules, path parsing)

### File (`File.h`, `File.cpp`)

Represents both files and directories using the `FileAttribute` enum:

```cpp
enum FileAttribute { FOLDER_ATT, FILE_ATT };
```

Features:
- Content storage for files
- Child management for directories
- Reader/writer tracking for concurrent access
- Mutex protection for thread safety

### Process Model

#### AbstractProcess (`AbstractProcess.h`, `AbstractProcess.cpp`)

Base class for all commands/programs. Implements:

- Thread-based execution
- Parameter handling
- I/O stream management
- Child process tracking
- Echo status per process

#### Shell (`Shell.h`, `Shell.cpp`)

The interactive command interpreter:

- Parses command lines
- Handles pipes and redirections
- Spawns child processes
- Provides help functionality

### I/O System

The I/O architecture follows a classic OOP hierarchy with abstract base classes:

```
AbstractIO (abstract base)
    ├── AbstractInput
    │   ├── StandardInput
    │   ├── FileInput
    │   └── PipeInput
    │
    └── AbstractOutput
        ├── StandardOutput
        ├── FileOutput
        └── PipeOutput
```

This design mirrors Java's stream hierarchy and allows polymorphic I/O handling.

#### AbstractIO (`AbstractIO.h`)

Base class holding a reference to the Kernel:

```cpp
class AbstractIO {
protected:
    Kernel* kernel;
};
```

#### AbstractInput / AbstractOutput

Define the interface for reading and writing:
- `ReadChar()` / `Write()` 
- `ReadLine()` / `WriteLine()`
- `Close()`

### Pipe (`Pipe.h`, `Pipe.cpp`)

Implements inter-process communication:

- Thread-safe buffer using `deque<char>`
- Condition variables for blocking reads/writes
- Support for closing read/write ends independently
- Configurable buffer size (default 64KB)

### Commands

Each command is a separate class extending `AbstractProcess`:

| Class | Command | Description |
|-------|---------|-------------|
| `Echo` | `echo` | Output text |
| `ChangeDirectory` | `cd` | Change working directory |
| `Dir` | `dir` | List directory contents |
| `WordCount` | `wc` | Count lines/words/characters |
| `MakeDirectory` | `md` | Create directory |
| `Remove` | `rd` | Remove directory |
| `Type` | `type` | Display file contents |
| `Sort` | `sort` | Sort input lines |
| `Rand` | `ranr` | Generate random numbers |
| `Freq` | `fraq` | Frequency analysis |
| `FirstLineFromFile` | `head` | Display first line |

## Design Patterns Used

### Factory Pattern

The `Kernel` acts as a factory for creating I/O and process objects:

```cpp
AbstractInput* CreateInputClass(IOType type, string param, ...);
AbstractOutput* CreateOutputClass(IOType type, string param, ...);
AbstractProcess* CreateProcessClass(string programName, ...);
```

### Template Method

`AbstractProcess` defines the skeleton of execution:

```cpp
int Run();           // Template method
virtual int RunProcess() = 0;  // Implementation
```

### Singleton (implicit)

The `Kernel` is instantiated once in `main()` and passed around via pointers, similar to a global service locator.

## Java-to-C++ Translation Artifacts

The Java background is evident in several code patterns:

### 1. Header-Only Class Definitions

Many classes have implementation in header files (like Java's inline definitions):

```cpp
class AbstractProcess {
    virtual string GetHelpContent() {
        return "Help text";  // Inline implementation
    }
};
```

### 2. Excessive Use of `using namespace std;`

The directive appears in headers, which is considered bad practice in C++:

```cpp
// In header files
using namespace std;
```

### 3. Raw Pointers Everywhere

No smart pointers; all ownership is managed via raw pointers (Java style).

### 4. Magic Numbers

Error codes use raw integers instead of enums or constants.

### 5. `#define EOF 26`

Redefines standard `EOF` macro, causing potential conflicts.

### 6. Public Member Variables

Some classes expose public members directly (uncommon in idiomatic C++).

## Threading Model

- Each process runs in its own `std::thread`
- The Kernel manages process lifecycle
- `Pipe` uses condition variables for producer-consumer synchronization
- `File` uses mutexes for concurrent read/write access

## Memory Management

**Note**: The codebase has several memory management issues (documented in `ISSUES.md`):

- Missing destructors in `Kernel`, `FileSystem`, `File`
- No smart pointer usage
- Potential thread leaks in `AbstractProcess::Run()`

## Build System

- **Windows**: Visual Studio project (`kivos.vcxproj`)
- **Linux/macOS**: Manual `g++` compilation with `-pthread` flag

## Extension Points

To add a new command:

1. Create `MyCommand.h` and `MyCommand.cpp`
2. Extend `AbstractProcess`
3. Implement `HasValidParameters()` and `RunProcess()`
4. Register in `Kernel::CreateProcessClass()`

## References

- `ISSUES.md`: Known bugs and improvement opportunities
- `doc/ClassModel.pdf`: UML class diagram
