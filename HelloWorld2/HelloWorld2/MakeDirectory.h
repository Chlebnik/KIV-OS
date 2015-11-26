#pragma once
class MakeDirectory : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool dirExists(const string& dirName_in);
	int processDirs(string drive, vector<string> sepDirs);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};

