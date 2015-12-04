#pragma once
class Echo : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	bool showStatus;
	int setEchoStatus = -1;

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};
