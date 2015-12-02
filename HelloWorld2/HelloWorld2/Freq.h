#pragma once

class Freq : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};

