#pragma once

class Dir : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool showAll;
	int pathIndex;
	string getTime(FILETIME time);
	int listDir(string path);


public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};

