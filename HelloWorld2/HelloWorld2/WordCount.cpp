#include "stdafx.h"

string WordCount::GetHelpContent()
{
	string help = "Syntax\n      wc [options]...[file]...\n\n		Options\n\n		- c\n		Print only the byte counts.\n\n		- w\n		Print only the word counts.\n\n		- l\n		Print only the newline counts.";
	return help;
}

bool WordCount::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;
	}
	else {
		for (std::vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (i == 0 && *it == "--help" && parameters.size() == 1) {
				showHelp = true;
				valid = true;
			}
			else if (i == 0 && *it == "-c" && parameters.size() < 3) {
				valid = true;
				countBytes = true;
				countWords = false;
				countLines = false;
				showHelp = false;
			}else if (i == 0 && *it == "-w") {
				valid = true;
				countBytes = false;
				countWords = true;
				countLines = false;
				showHelp = false;
			}
			else if (i == 0 && *it == "-l") {
				valid = true;
				countBytes = false;
				countWords = false;
				countLines = true;
				showHelp = false;
			}else if(parameters.size() == 2 && (countBytes || countWords || countLines)){
				fromFile = true;
			}
			i++;
		}
	}

	return valid;
}

void WordCount::getCount(AbstractInput* processInput) {
	bool success = true;
	string line;
	while (processInput->HasNext()) {
		line = processInput->ReadLine(success);
		if (success) {
			if (countBytes) {
				std::vector<char> bytes(line.begin(), line.end());
				bytes.push_back('\0');
				count += static_cast<int>(bytes.size());
			}
			else if (countWords) {
				istringstream iss(line);
				vector<string> words{ istream_iterator<string>{iss},
					istream_iterator<string>{} };
				count += static_cast<int>(words.size());
			}
			else if (countLines) {
				count += 1;
			}
		}
	}

	if (countBytes) {
		output->WriteLine("Bytes count is :" + to_string(count));
	}
	else if (countWords) {
		output->WriteLine("Words count is :" + to_string(count));
	}
	else if (countLines) {
		output->WriteLine("Lines count is :" + to_string(count));
	}
}

int WordCount::RunProcess()
{
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}else if (fromFile) {
			AbstractInput* fileFromParam = NULL;
			shared_ptr<ifstream> fileInput(new ifstream(parameters[1]));
			if (!*fileInput)
			{
				return -1;
			}			
			fileFromParam = new FileInput(fileInput, kernel);

			getCount(fileFromParam);
			fileFromParam->Close();
	}
	else {
		getCount(input);
	}
		



	output->Close();
	input->Close();	

	return 0;
}
