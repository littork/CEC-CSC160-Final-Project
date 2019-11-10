#pragma once

#include <string>

class Autoupdate {
private:
	Autoupdate() {};
public:
	static void begin();
private:
	static std::string buildNumber;
};