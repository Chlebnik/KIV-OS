#pragma once



class ChangeDirectory : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool changeDrive;
	int pathIndex;
	string read_line();
	void write_line(string line);
	bool dirExists(const string& dirName_in);
	bool changePath(string new_path);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();
};

