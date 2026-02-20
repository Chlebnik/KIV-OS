#include "stdafx.h"
#include "Shell.h"
#include "Kernel.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

// Helper to print process_data
void printCommand(const process_data& cmd) {
    cout << "  Name: " << cmd.process_name << endl;
    cout << "  Params: " << cmd.process_parameters << endl;
    cout << "  Input: " << cmd.process_input.type << ", '" << cmd.process_input.path << "'" << endl;
    cout << "  Output: " << cmd.process_output.type << ", '" << cmd.process_output.path << "'" << endl;
}

void test_parseInput() {
    Kernel mockKernel;
    // Shell constructor: pid=1, parentPid=0, kernel=&mockKernel
    Shell shell(1, 0, &mockKernel);

    struct TestCase {
        string input;
        int expectedReturn;
        vector<process_data> expectedCommands;
    };

    vector<TestCase> cases = {
        // 1. Simple command
        {
            "ls",
            0,
            { {"ls", "", {0, ""}, {0, ""}} }
        },
        // 2. Command with params
        {
            "ls -l",
            0,
            { {"ls", "'-l'", {0, ""}, {0, ""}} }
        },
        // 3. Command with quotes
        {
            "echo \"hello world\"",
            0,
            { {"echo", "'hello world'", {0, ""}, {0, ""}} }
        },
        // 4. Input redirection
        {
            "cat < input.txt",
            0,
            { {"cat", "", {1, "input.txt"}, {0, ""}} }
        },
        // 5. Output redirection
        {
            "ls > output.txt",
            0,
            { {"ls", "", {0, ""}, {1, "output.txt"}} }
        },
        // 6. Pipe
        {
            "ls | grep foo",
            0,
            {
                {"ls", "", {0, ""}, {0, ""}},
                {"grep", "'foo'", {0, ""}, {0, ""}}
            }
        },
        // 7. Multiple params
        {
            "cmd p1 p2",
            0,
            { {"cmd", "'p1' 'p2'", {0, ""}, {0, ""}} }
        },
        // 8. Empty input
        {
            "",
            0,
            {}
        },
        // 9. Spaces only
        {
            "   ",
            0,
            {}
        },
        // 10. Complex command
        {
            "sort < in.txt | grep \"pattern\" > out.txt",
            0,
            {
                {"sort", "", {1, "in.txt"}, {0, ""}},
                {"grep", "'pattern'", {0, ""}, {1, "out.txt"}}
            }
        },
        // 11. Invalid syntax: redirect without space (based on code analysis)
        {
            "ls >out",
            -1,
            {}
        },
        // 12. Invalid syntax: pipe start
        // Shell::parseInput checks: if input.empty() ... then ParseProgram.
        // ParseProgram checks for pipe immediately?
        // ParseProgram: if (ch == OUTPUT_REDIRECT) ... return -1.
        {
            "| ls",
            -1,
            {}
        }
    };

    int passed = 0;
    int failed = 0;

    for (size_t i = 0; i < cases.size(); ++i) {
        const auto& tc = cases[i];
        cout << "Test Case " << i + 1 << ": '" << tc.input << "'" << endl;

        vector<process_data> commands;
        int index = 0;
        int result = shell.parseInput(tc.input, index, &commands);

        bool localPass = true;
        if (result != tc.expectedReturn) {
            cout << "  FAILED: Expected return " << tc.expectedReturn << ", got " << result << endl;
            localPass = false;
        } else if (result == 0) {
            if (commands.size() != tc.expectedCommands.size()) {
                 cout << "  FAILED: Expected " << tc.expectedCommands.size() << " commands, got " << commands.size() << endl;
                 localPass = false;
            } else {
                for (size_t j = 0; j < commands.size(); ++j) {
                    const auto& actual = commands[j];
                    const auto& expected = tc.expectedCommands[j];

                    if (actual.process_name != expected.process_name ||
                        actual.process_parameters != expected.process_parameters ||
                        actual.process_input.type != expected.process_input.type ||
                        actual.process_input.path != expected.process_input.path ||
                        actual.process_output.type != expected.process_output.type ||
                        actual.process_output.path != expected.process_output.path) {
                        cout << "  FAILED: Command " << j << " mismatch." << endl;
                        cout << "  Expected:" << endl;
                        printCommand(expected);
                        cout << "  Actual:" << endl;
                        printCommand(actual);
                        localPass = false;
                    }
                }
            }
        }

        if (localPass) {
            cout << "  PASSED" << endl;
            passed++;
        } else {
            failed++;
        }
    }

    cout << "\nSummary: " << passed << " passed, " << failed << " failed." << endl;

    if (failed > 0) exit(1);
}

int main() {
    test_parseInput();
    cout << "Tests finished. Exiting main." << endl;
    return 0;
}
