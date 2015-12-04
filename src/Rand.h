#pragma once
#include "stdafx.h"

class Rand : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;

private:
	bool showHelp;
	atomic_bool stop;
	void Loop();
	string convertFloatToString(float number);

public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};