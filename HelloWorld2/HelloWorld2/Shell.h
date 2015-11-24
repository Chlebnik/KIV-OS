#pragma once
#include "stdafx.h"

class Shell : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
public:
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

private:
	int ProcessInput(string line);
};