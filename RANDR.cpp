#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <stdlib.h> 
#include <random>
#include <functional>
#include <string>
#include <thread>

using namespace std;

bool run;

void write_line(string line) {

	cout << line << endl;
}

string read_line() {
	string line;
	getline(cin, line);

	return line;
}

string convertFloatToString(float number) {
	ostringstream buff;
	buff << number;
	return buff.str();
}
int printRandomNumber() {

	//Mersenne Twister 19937 generato
	mt19937 generator;
	//Uniform real distribution with mean 0 and deviation 0
	uniform_real_distribution<float> distribution(-1, 1);

	//binding generator to function for repeated use
	auto dice = bind(distribution, generator);

	// until thread have to end write random float to STDOUT
	string line;
	while (run) {
		line = convertFloatToString(dice());
		write_line(line);
	}

	return 0;
}

//Parameters validation
bool checkParams(int argc, char* argv[]) {
	bool valid = true;
	for (int i = 1; i < argc; i++){
		string checkValue = argv[i];
		if (checkValue == "--help") {
			write_line("Program is printing random float numbers to STDOUT until CTRL+Z is given as input");
			exit(EXIT_SUCCESS);
		}else {
			write_line("Invalid parameter :"+ checkValue);
			valid = false;
		}
	}

	return valid;
}

int main(int argc, char* argv[])
{	
	if (checkParams(argc, argv)) {

		run = true;

		thread t1(printRandomNumber);

		// until EOF is reached read from STDIN
		string line;
		while (!cin.eof()) {
			line = read_line();
		}

		// Kill thread
		run = false;
		t1.join();
	}
	return 0;
}

