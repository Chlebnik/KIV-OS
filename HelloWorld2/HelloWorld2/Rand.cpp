#include "stdafx.h"
using namespace std;


string Rand::GetHelpContent()
{
	string help = "Program is printing random float numbers to STDOUT until CTRL+Z is given as input";
	return help;
}

bool Rand::HasValidParameters()
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

string Rand::convertFloatToString(float number) {
	ostringstream buff;
	buff << number;
	return buff.str();
}

void Rand::write_line(string line) {
	output->WriteLine(line);

}

string Rand::read_line() {
	string line;
	bool success = true;

	line = input->ReadLine(success);

	if (success) {
		return line;
	}
}

int Rand::RunProcess()
{
	if (showHelp) {
		write_line(GetHelpContent());
	}
	else {
		stop = false;

		thread t1(&Rand::Loop, this);

		// until EOF is reached read from STDIN
		char inputChar;
		cin.clear();
		while (cin)
		{
			cin.get(inputChar);
		}
		cin.ignore();

		// Kill thread
		stop = true;
		t1.join();
	}
		
	output->Close();
	input->Close();
	return 0;
}

void Rand::Loop()
{
	//Mersenne Twister 19937 generato
	mt19937 generator;
	//Uniform real distribution with mean 0 and deviation 0
	uniform_real_distribution<float> distribution(-1, 1);

	//binding generator to function for repeated use
	auto dice = bind(distribution, generator);

	string line;
	while (!stop)
	{
		line = convertFloatToString(dice());
		write_line(line);
		Sleep(1000);
	}

}