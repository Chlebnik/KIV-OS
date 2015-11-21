#include "stdafx.h"

using namespace std;

string Shell::GetHelpContent()
{
	return "Shell does not need any help... :)";
}

bool Shell::HasValidParameters()
{
	return true;
}

int Shell::RunProcess()
{
	string line;
	while (true)
	{		
		output->Write("prompt:>");
		bool success = true;
		line = input->ReadLine(success);
		if (line == "exit")
		{
			break;
		}
		this->ProcessInput(line);
	}
	return 0;
}

int Shell::ProcessInput(string line)
{
	bool insideQuotes = false;
	bool mustFollowSpace = false;
	bool wasSpaceBefore = false;
	string param = "";
	vector<string> parameters {};
	int size = 0;

	for (string::iterator it = line.begin(); it != line.end(); ++it)
	{
		char c = *it;
		
		if (insideQuotes && c != '\"')
		{
			param += c;
			continue;
		}	

		switch (c)
		{
		case ' ':
			if (param.size() > 0)
			{
				parameters.resize(parameters.size() + 1);
				parameters[parameters.size() - 1] = param;
			}
			mustFollowSpace = false;
			wasSpaceBefore = true;
			param = "";
			break;
		case '\"':
			if (insideQuotes)
			{
				mustFollowSpace = false;
				insideQuotes = false;
			}
			else
			{
				if (!wasSpaceBefore && parameters.size() > 0)
				{
					return 12; // space must be preceding
				}
				insideQuotes = true;
			}
			
			break;
		case '|':
		case '>':
		case '<':
			if (!wasSpaceBefore)
			{
				return 12; // space has to be before pipe character
			}
			parameters.resize(parameters.size() + 1);
			parameters[parameters.size() - 1] = "|";
			mustFollowSpace = true;
			break;
		default:
			param += c;
		}

	}

	if (insideQuotes)
	{
		return 13; // Quotes not ended
	}

	if (param.size() > 0)
	{
		parameters.resize(parameters.size() + 1);
		parameters[parameters.size() - 1] = param;
	}

	for (vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		output->WriteLine(*it);
	}
	output->WriteLine("");
	return 0;
}

