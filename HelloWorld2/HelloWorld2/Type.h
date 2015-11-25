#pragma once

class Type : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool fromFile;
	bool fromRegex;
	int count;
	string getBasename(string filepath);
	void proccesFile(string filepath);
	void printFileContent(AbstractInput* printInput);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};