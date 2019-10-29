#pragma once

#include <string>

class JSONParser {
public:
	JSONParser(const std::string& _path);
private:
	std::string path;
};