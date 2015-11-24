#pragma once
class Echo : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
private:
	bool showHelp;
	void write_line(string line);
	string read_line();


public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};
