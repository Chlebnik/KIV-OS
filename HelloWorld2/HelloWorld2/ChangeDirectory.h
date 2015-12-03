#pragma once



class ChangeDirectory : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool changeDrive;
	int pathIndex;
	bool DirExists(const string& dirName_in);
	int ChangePath(string new_path);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};

