#include "stdafx.h"
using namespace std;


Rand::Rand(int pid, Kernel* kernel) : AbstractProcess(pid, kernel), stop{false}{}

string Rand::GetHelpContent()
{
	return "TODO - sort help";
}

bool Rand::HasValidParameters()
{
	return true;
}

int Rand::RunProcess()
{
	thread t(&Rand::Loop, this);

	char inputChar;
	cin.clear();
	//input->Read();
	while (cin)
	{
		cin.get(inputChar);
	}
	cin.ignore();
	stop = true;
	t.join();
	output->Close();
	
	return 0;
}

void Rand::Loop()
{
	while (!stop)
	{
		int number = rand();
		string s = to_string(number);
		output->WriteLine(s);
		Sleep(1000);
	}
}