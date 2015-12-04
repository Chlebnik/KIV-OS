#pragma once



class ChangeDirectory : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool changeDrive;
	int pathIndex;
	int ChangePath(string new_path);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};

