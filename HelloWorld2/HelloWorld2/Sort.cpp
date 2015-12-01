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

string Sort::GetSortedLine() {
	vector<FirstLineFromFile> sortedLines = firstLinesOfOpenFiles;
	string line;
	
	sort(sortedLines.begin(), sortedLines.end(), FirstLineFromFile::FirstOperator);
	bool succes;
	line = sortedLines.back().GetLine(succes);

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
	}


	return line;
}

int Sort::ProcessAray(vector<string>* array) {
	sort(array->begin(), array->end(), FirstLineFromFile::FirstOperatorString);
	string path = "tempSort_" + to_string(1 + firstLinesOfOpenFiles.size()) + ".txt";

	shared_ptr<ofstream> fileOutput(new ofstream(path));
	if (!*fileOutput) {
		//Can not open file
		return -5;
	}

	AbstractOutput* temp_out = new FileOutput(fileOutput, kernel);
	while (array->size() > 0) {
		temp_out->WriteLine(array->back());
		array->pop_back();
	}
	temp_out->Close();
	delete temp_out;

	firstLinesOfOpenFiles.push_back(FirstLineFromFile(firstLinesOfOpenFiles.size(), path, kernel));


	return 0;
}

int Sort::RunProcess()
{	

	int result = 0;
	vector<string> array {};

	while (input->HasNext())
	{
		string line;
		bool success = true;
		line = input->ReadLine(success);

		if (success)
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
			output->WriteLine(GetSortedLine());
		}
	}
	
	return 0;
}