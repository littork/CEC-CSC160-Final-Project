#pragma once

#include <string>

class Shell {
private:
	Shell() {};
public:
	static std::string run(const char* command);
};