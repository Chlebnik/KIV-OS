#pragma once

class Sort : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;

private:
	bool showHelp;
	vector<FirstLineFromFile> firstLinesOfOpenFiles;
	int ProcessAray(vector<string>* Array);
	string GetSortedLine();
	int maxByteSize = 1;
public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};

