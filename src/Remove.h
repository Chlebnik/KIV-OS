#pragma once
class Remove : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;

private:
	bool showHelp;
	bool force;
	int pathIndex;
	bool RemoveFile(File* file);
	int maxByteSize = 1;
public:
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};

