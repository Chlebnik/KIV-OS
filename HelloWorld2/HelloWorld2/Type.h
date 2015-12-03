#pragma once

class Type : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool fromFile;
	bool fromRegex;
	int count;
	int listDir(string path, string regexFile);
	int proccesFile(string filepath);
	int printFileContent(AbstractInput* printInput);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};