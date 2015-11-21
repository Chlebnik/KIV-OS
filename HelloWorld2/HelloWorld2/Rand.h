#pragma once
#include "stdafx.h"

class Rand : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;

private:
	atomic_bool stop;
	void Loop();

public:
	Rand(int pid, Kernel* kernel);
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};