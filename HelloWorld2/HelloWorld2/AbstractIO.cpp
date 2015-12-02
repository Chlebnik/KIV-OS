#include "stdafx.h"

AbstractIO::AbstractIO(shared_ptr<Kernel> kernel) : kernel{ kernel }
{

}

shared_ptr<Kernel> AbstractIO::GetKernel() {
	return kernel;
}