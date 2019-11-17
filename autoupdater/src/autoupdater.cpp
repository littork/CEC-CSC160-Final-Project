#include <iostream>
#include <string>

#include <Windows.h>

#include <curl/curl.h>

#include <iostream>
#include <fstream>

size_t FileWriteCallback(char* buf, size_t size, size_t nmemb, void* up) {
	size_t written = fwrite(buf, size, nmemb, ( FILE*) up);
	return written;
}

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

	// Wait for interface to close and unlock
	Sleep(2500);

	const std::string targetPath = argv[1];
	const std::string targetURL = argv[2];

	std::cout << "Target path: " << targetPath << std::endl;
	std::cout << "Target url: " << targetURL << std::endl;

	{ // Delete old interface
		std::cout << "Removing old interface executable\n";
		std::remove(targetPath.c_str());
	}

	{ // Download new interface
		std::cout << "Downloading new interface executable\n";
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, targetURL.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, -1);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &FileWriteCallback);

		FILE* autoupdateFile = fopen(targetPath.c_str(), "wb");
		if (autoupdateFile) {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, autoupdateFile);
			CURLcode res = curl_easy_perform(curl);

			char autoUpdaterError[CURL_ERROR_SIZE];
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, autoUpdaterError);
			autoUpdaterError[0] = 0;

			fclose(autoupdateFile);
			if (res != CURLE_OK) {
				std::cout << "Failed to download update: ";
				std::cout << autoUpdaterError << std::endl;
				return true;
			}
		} else {
			std::cout << "Failed to download update: ";
			std::cout << "Unable to open local file to write downloaded data to\n";
			curl_easy_cleanup(curl);
			return true;
		}

		curl_easy_cleanup(curl);
	}

	{ // Restart interface
		std::cout << "Restarting interface\n";
		STARTUPINFO si = {sizeof(STARTUPINFO)};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		std::string cmd = std::string(targetPath);
		CreateProcess(cmd.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}

	return 0;
}