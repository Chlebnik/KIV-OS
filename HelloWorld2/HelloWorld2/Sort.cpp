#include "stdafx.h"
using namespace std;

string Sort::GetHelpContent()
{
	return "TODO - sort help";
}

bool Sort::HasValidParameters()
{
	return true;
}

int Sort::RunProcess()
{
	//cout << "test";
	vector<string> array {};
	while (input->HasNext())
	{
		string line;
		bool success = true;
		line = input->ReadLine(success);
		//cout << line;
		if (success)
		{
			array.resize(array.size() + 1);
			array[array.size() - 1] = line;
		}		
	}

	sort(array.begin(), array.end());

	for (vector<string>::iterator it = array.begin(); it != array.end(); ++it)
	{
		cout << *it;
		output->WriteLine(*it);
	}

	return 0;
}