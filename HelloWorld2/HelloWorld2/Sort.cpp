#include "stdafx.h"
using namespace std;

string Sort::GetHelpContent()
{
	string help = "Sorts input and writes it to output.";
	return help;
}

bool Sort::HasValidParameters()
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

string Sort::GetSortedLine(int& response) {
	vector<FirstLineFromFile> sortedLines = firstLinesOfOpenFiles;
	string line;
	
	sort(sortedLines.rbegin(), sortedLines.rend(), FirstLineFromFile::FirstOperator);
	bool succes;
	line = firstLinesOfOpenFiles.at(sortedLines.back().GetIndex()).GetLine(succes);

	if (!succes) {
		vector<FirstLineFromFile> tmp;
		for (int i = 0; i != firstLinesOfOpenFiles.size(); ++i)
		{
			if (i != sortedLines.back().GetIndex()) {
				tmp.push_back(firstLinesOfOpenFiles[i]);
			}
		}
		firstLinesOfOpenFiles.clear();
		firstLinesOfOpenFiles = tmp;
		string path = "_system:/tempSort_" + to_string(sortedLines.back().GetIndex()+1) + ".txt";
		tmp.clear();
		sortedLines.clear();
		response = kernel->RemoveFile(path);
	}
	else {
		response = 0;
	}


	return line;
}

int Sort::ProcessAray(vector<string>* array) {
	sort(array->begin(), array->end(), FirstLineFromFile::FirstOperatorString);
	string path = "_system:/tempSort_" + to_string(1 + firstLinesOfOpenFiles.size()) + ".txt";
	int response = 0;
	AbstractOutput* temp_out = kernel->CreateOutputClass(FILE_TYPE, path, this->GetPid(), GetPathFile(), response);
	while (array->size() > 0) {
		temp_out->WriteLine(array->back());
		array->pop_back();
	}
	temp_out->Close();
	delete temp_out;

	firstLinesOfOpenFiles.push_back(FirstLineFromFile(firstLinesOfOpenFiles.size(), path, kernel, this->GetPid(), this->GetPathFile()));


	return 0;
}

int Sort::RunProcess()
{	

	int result = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else {

		vector<string> array {};

		while (input->HasNext())
		{
			string line;
			bool success = true;
			line = input->ReadLine(success);

			if (success || line.length() > 0)
			{
				array.push_back(line);

				if (kernel->QueryLowMemoryStatus()) {
					result = ProcessAray(&array);
					if (result != 0) {
						return result;
					}
				}
			}
		}
		if (array.size() > 0) {
			result = ProcessAray(&array);
			if (result != 0) {
				return result;
			}
		}

		if (firstLinesOfOpenFiles.size() > 0) {
			while (firstLinesOfOpenFiles.size() > 0)
			{	
				int response;
				string line = GetSortedLine(response);
				if (response != 0) {
					return response;
				}
				output->WriteLine(line);
			}
		}
	}
	output->Close();
	return result;
}