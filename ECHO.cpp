#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;

void write_line(string line) {

	cout << line << endl;
}

string read_line() {
	string line;
	getline(cin, line);

	return line;
}

//Parameters validation
bool checkParams(int argc, char* argv[]) {
	bool valid = true;
	for (int i = 1; i < argc; i++) {
		string checkValue = argv[i];
		if (checkValue == "--help") {
			write_line("Displays messages, or turns command-echoing on or off.\n\nECHO [ON | OFF]\nECHO[message]\n\nType ECHO without parameters to display the current echo setting.");
			exit(EXIT_SUCCESS);
		}
		else {
			write_line(checkValue);
		}
	}

	return valid;
}

int main(int argc, char* argv[])
{
	
	return checkParams(argc, argv);
}
