// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Kernel.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

using namespace std;

Kernel kernel;

void write_line(string line) {

	cout << line << endl;
}

string read_line() {
	string line;
	getline(cin, line);

	return line;
}

// Metoda zùstane, ale bude volat kernelovskou funkci
bool dirExists(const string& dirName_in)
{
	// Tohle se pøesune do KERNELU
	DWORD ftyp = kernel.OurGetFileAttributesA(dirName_in);

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool changePath(string new_path, bool changeDir) {
	bool success = false;

	if (dirExists(new_path)) {
		wchar_t* wstr = kernel.StringToWchar(new_path);

		if (!SetCurrentDirectory(wstr))
		{
			string error = "SetCurrentDirectory failed " + GetLastError();
			write_line(error);
		}
		system("Dir");
		delete[] wstr;		
	}
	else {
		write_line("Given path doesn't exist!");
	}

	return success;
}

//Parameters validation
bool checkParams(int argc, char* argv[]) {
	bool valid = true;
	bool changeDrive = false;

	if (argc == 1) {
		write_line("No parameter was given");
		valid = false;
	}
	else if (argc > 3) {
		write_line("Too much parameter were given");
		valid = false;
	}
	else{
		for (int i = 1; i < argc; i++) {
			string checkValue = argv[i];
			if (checkValue == "--help") {
				write_line("Displays the name of or changes the current directory.\n\nCD[/ D][drive:][path]\nCD[..]\n\n..Specifies that you want to change to the parent directory.\n\nType CD drive : to display the current directory in the specified drive.\nType CD without parameters to display the current drive and directory.\n\nUse the / D switch to change current drive in addition to changing current\ndirectory for a drive.\n\nIf Command Extensions are enabled CHDIR changes as follows :\n\n			The current directory string is converted to use the same case as\n					the on disk names.So CD C : \TEMP would actually set the current\n					directory to C : \Temp if that is the case on disk.");
				exit(EXIT_SUCCESS);				
			}
			else if(checkValue.compare(0,2,"/D") == 0) {
				changeDrive = true;
			}
			else {
				valid = changePath(checkValue, changeDrive);
			}
		}
	}

	return valid;
}

int main(int argc, char* argv[])
{	
	kernel = Kernel();
    return (checkParams(argc, argv));
}

