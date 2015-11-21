#pragma once
#include "stdafx.h"

class Sort : public AbstractProcess
{
	using AbstractProcess::AbstractProcess;
public:
	// Inherited form Abstract process
	string GetHelpContent();
	bool HasValidParameters();
	int RunProcess();

};
