// IOutput: defines interface for Outputs
//
#include <mutex>
#include <string>
#include "Kernel.h"

class IOutput
{
public:
	virtual ~IOutput() {}
	virtual int close() = 0;
	virtual int write(std::string) = 0;
	virtual int write_line(std::string) = 0;


protected:
	// Added for parralel processing one Output
	std::mutex lock;
	// Added for manipulation with OS 
	Kernel kernel;
};