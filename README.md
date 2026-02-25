# KIV/OS

A minimalistic operating system simulation written in C++. The project was originally developed by developers with a Java background, which significantly influenced its architecture and design patterns.

## Overview

KIV/OS simulates a basic operating system with the following features:

- **File System**: Virtual RAM disk with drive-based directory structure
- **Shell**: Command-line interface supporting pipes, input/output redirection
- **Built-in Commands**: echo, cd, dir, wc, md, rd, type, sort, ranr, fraq

## Supported Commands

| Command | Description |
|---------|-------------|
| `echo` | Display text |
| `cd` | Change directory |
| `dir` | List directory contents |
| `wc` | Word count |
| `md` | Make directory |
| `rd` | Remove directory |
| `type` | Display file contents |
| `sort` | Sort lines |
| `ranr` | Random number generation |
| `fraq` | Frequency analysis |
| `freq` | Character frequency |
| `head` | First line from file |

## Building

### Linux/macOS

```bash
cd /path/to/KIV-OS/src
g++ -std=c++11 -pthread -o kivos *.cpp
```

### Windows (Visual Studio)

Open `kivos.vcxproj` in Visual Studio and build.

## Usage

Run the executable:

```bash
./kivos
```

The shell will start with a command prompt. Enter commands similar to DOS/Windows:

```
C:\> echo Hello World
C:\> cd c:\
C:\> dir
C:\> type myfile.txt
C:\> echo Hello | sort
```

## Project Structure

```
KIV-OS/
├── src/
│   ├── kivos.cpp          # Main entry point
│   ├── Kernel.*           # Core OS kernel
│   ├── FileSystem.*       # Virtual file system
│   ├── Shell.*            # Command shell
│   ├── Pipe.*             # Inter-process pipes
│   ├── File.*             # File/directory representation
│   ├── Abstract*.h       # Abstract base classes
│   └── [Commands]         # Individual command implementations
├── tests/                 # Test files
├── doc/                   # Documentation
├── README.txt            # Original readme
└── ISSUES.md             # Known issues
```

## Features

- **Pipes**: Connect commands with `|`, e.g., `echo hello | sort`
- **Redirection**: Use `<` for input and `>` for output
- **RAM Disk**: In-memory file system with drive letters
- **Multi-threading**: Concurrent process execution

## Notes

This project was created by developers with a Java background, resulting in extensive use of OOP patterns (inheritance, abstract classes, factory patterns) that are characteristic of Java development but may feel unusual in C++ code.
