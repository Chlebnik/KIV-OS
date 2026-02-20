#include "stdafx.h"
#include "ChangeDirectory.h"
#include <iostream>
#include <string>

using namespace std;

int g_fail_count = 0;

void run_test(string params, bool expected, string message) {
    ChangeDirectory cd(0, 0, nullptr); // Kernel nullptr is safe for HasValidParameters
    // Use Init to set parameters through AbstractProcess logic
    // We pass nullptr for input/output/errorOutput as they are not used in HasValidParameters
    cd.Init(nullptr, nullptr, nullptr, params);

    bool result = cd.HasValidParameters();

    if (result == expected) {
        cout << "[PASS] " << message << " (params: \"" << params << "\")" << endl;
    } else {
        cout << "[FAIL] " << message << " (params: \"" << params << "\")" << endl;
        cout << "  Expected: " << (expected ? "true" : "false") << ", Got: " << (result ? "true" : "false") << endl;
        g_fail_count++;
    }
}

int main() {
    cout << "Running ChangeDirectory tests..." << endl;
    cout << "Note: Parameters must be wrapped in single quotes as per Shell -> AbstractProcess protocol." << endl;

    // Test cases based on code analysis and expected behavior

    // 1. Empty parameters
    // Expect: Valid (Show Help)
    run_test("", true, "Empty parameters");

    // 2. --help
    // Expect: Valid (Show Help)
    run_test("'--help'", true, "--help argument");

    // 3. Simple path
    // Expect: Valid
    run_test("'C:\\Windows'", true, "Simple path");

    // 4. Path with spaces (Simulating Shell processing: "C:\Program Files" -> 'C:\Program Files')
    // Expect: Valid
    run_test("'C:\\Program Files'", true, "Path with spaces");

    // 5. /D switch with path
    // Expect: Valid
    run_test("'/D' 'C:\\Windows'", true, "/D switch with path");

    // 6. /D switch without path
    // Expect: Invalid (Should specify path)
    run_test("'/D'", false, "/D switch only");

    // 7. Invalid parameters (Two paths)
    // Expect: Invalid
    run_test("'path1' 'path2'", false, "Two paths");

    // 8. Invalid switch (treated as path)
    // Expect: Valid (Treated as directory named '/Invalid')
    run_test("'/Invalid'", true, "Invalid switch treated as path");

    // 9. Extra arguments with /D
    // Expect: Invalid
    run_test("'/D' 'path' 'extra'", false, "Extra arguments with /D");

    if (g_fail_count > 0) {
        cout << "\nTests Failed: " << g_fail_count << endl;
        return 1;
    }
    cout << "\nAll tests passed!" << endl;
    return 0;
}
