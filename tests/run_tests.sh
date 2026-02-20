#!/bin/bash
set -e

# Create tests directory if it doesn't exist
mkdir -p tests

# Compile the test runner
# We include 'tests' first so that 'stdafx.h' in tests/ is picked up instead of src/stdafx.h if included with ""
# We define TESTING_ENV to use the test configuration in src/stdafx.h
# We use -fpermissive because legacy C++ code might need it (as suggested by memory)
# We don't link src/Utils.cpp because of Windows dependencies, relying on mock or unused symbols.
g++ -I tests -I src \
    tests/test_ChangeDirectory.cpp \
    src/ChangeDirectory.cpp \
    src/AbstractProcess.cpp \
    -o tests/test_runner \
    -DTESTING_ENV \
    -std=c++11 -pthread -fpermissive

# Run the tests
./tests/test_runner
