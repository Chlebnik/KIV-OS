#pragma once
class WordCount : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool countBytes;
	bool countWords;
	bool countLines;
	bool fromFile;
	int count;

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};

