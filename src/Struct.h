#include <string>

struct process_parameter {
	std::string name;
	std::string value;
};

struct process_input {
	int type;
	std::string path;
};

struct process_output {
	int type;
	std::string path;
};

struct process_data {
	std::string process_name;
	std::string process_parameters;
	process_input process_input;
	process_output process_output;
};

