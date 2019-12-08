#pragma once

#include <string>

class Autoupdate {
private:
	Autoupdate() {};
public:
	static bool check();
	static void cleanup(const std::string& targetPath, const std::string& executable);

	static std::string stringPath();

	static std::string getRemoteBuildNumber();
};