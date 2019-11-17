#pragma once

#include <string>

class Autoupdate {
private:
	Autoupdate() {};
public:
	static bool check();
private:
	static void cleanup(const std::string& targetPath);
};