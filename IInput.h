// IInput: defines interface for Inputs
//
#include <string>
#include <mutex>
#include "Kernel.h"

class IInput
{
public:
	virtual ~IInput() {}
	virtual int close() = 0;
	virtual bool has_next() = 0;
	virtual std::string read_line() = 0;


protected:
	// Added for parralel processing one Input
	std::mutex lock;
	// Added for manipulation with OS 
	Kernel kernel;
};
