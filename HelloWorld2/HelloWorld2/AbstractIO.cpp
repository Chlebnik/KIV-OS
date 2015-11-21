#include "stdafx.h"

AbstractIO::AbstractIO(Kernel* kernel) : kernel{ kernel }
{

}

Kernel* AbstractIO::GetKernel() {
	return kernel;
}