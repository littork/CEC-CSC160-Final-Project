#include <iostream>
#include <string>

#include <Windows.h>

int main(int argc, char* argv[]) {
	const std::string deletePath = argv[1];
	const std::string updateURL = argv[2];

	if (!deletePath.size() || !updateURL.size()) {
		return -1;
	}

	std::cout << "Updating file: " << deletePath << std::endl;
	std::cout << "Retrieving update from: " << updateURL << std::endl;

	std::system(std::string("curl -L " + updateURL + " -o updated.exe").c_str());

	std::string app_path(4096, '\0');
	app_path.resize(GetModuleFileName(0, &app_path.front(), (DWORD) app_path.size()));
	std::system(std::string("del /Q /F \"" + deletePath + "\"").c_str());

	std::system(std::string("copy updated.exe \"" + deletePath + "\"").c_str());

	std::system(std::string("del /Q /F \"updated.exe\"").c_str());

	std::system("pause");

	return 0;
}