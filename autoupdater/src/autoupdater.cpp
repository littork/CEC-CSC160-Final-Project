#include <iostream>
#include <string>

#include <Windows.h>

#include <curl/curl.h>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	/*curl_global_init(CURL_GLOBAL_ALL);

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

	std::system("pause");*/

	std::cout << "Autoupdater: Beginning auto update\n";

	const std::string targetPath = argv[1];
	const std::string targetURL = argv[2];

	std::cout << "Target path: " << targetPath << std::endl;
	std::cout << "Target url: " << targetURL << std::endl;

	{ // Restart interface
		STARTUPINFO si = {sizeof(STARTUPINFO)};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		std::string cmd = std::string(targetPath);
		CreateProcess(cmd.c_str(), NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	}

	return 0;
}