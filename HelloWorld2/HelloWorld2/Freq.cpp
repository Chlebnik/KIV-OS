#include "stdafx.h"

string Freq::GetHelpContent()
{
	string help = "Displays histogram of bytes from given input which have count more then zero";
	return help;
}

bool Freq::HasValidParameters()
{
	bool valid = false;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = false;

	}
	else if (parameters.size() == 1 && parameters[0] == "--help") {
		valid = true;
		showHelp = true;
	}

	return valid;
}

int Freq::RunProcess() {
	returnVal = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else {
		bool success;
		string line;
		int bytesCount[MAXBYTE] = {};

		while (input->HasNext()) {
			line = input->ReadLine(success);
			if (success) {
				vector<char> bytes(line.begin(), line.end());
				for (char byte : bytes) {
					bytesCount[byte]++;					
				}
			}
			else {
				returnVal = -1;
				this->Close();
				return returnVal;
			}
		}
		for (int i = 0; i < MAXBYTE; i++) {
			if (bytesCount[i] > 0) {
				char buffer[50];
				sprintf_s(buffer, "0x%hhx : %d",i, bytesCount[i]);
				output->WriteLine(buffer);
			}
		}
	}
	this->Close();
	return returnVal;
}