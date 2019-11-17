#include "Autoupdate.h"

#include "Macros.h"

#include <Windows.h>
#include <cstdio>
#include <string>
#include <errno.h>

#include <nana/gui.hpp>
#include <curl/curl.h>

#ifndef BUILD_NUMBER
#define BUILD_NUMBER 0
#endif

size_t BlankWriteCallback(char* buf, size_t size, size_t nmemb, void* up) {
	return size * nmemb;
}

size_t FileWriteCallback(char* buf, size_t size, size_t nmemb, void* up) {
	size_t written = fwrite(buf, size, nmemb, (FILE*) up);
	return written;
}

void Autoupdate::cleanup(const std::string& targetPath) {
	std::string targetRemovePath(targetPath + AUTOUPDATER_EXECUTABLE);

	// Would expect an error if no file is present
	std::remove(targetRemovePath.c_str());
}

bool Autoupdate::check() {
	HMODULE hModule = GetModuleHandle(NULL);
	CHAR path[MAX_PATH];
	GetModuleFileName(hModule, path, MAX_PATH);

	std::string strPath(path);
	strPath = strPath.substr(0, strPath.find_last_of('\\\\')) + "\\";

	cleanup(strPath);

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, REPOSITORY_URL);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, -1);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &BlankWriteCallback);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

	char error[CURL_ERROR_SIZE];
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
	error[0] = 0;

	CURLcode res = curl_easy_perform(curl);
	
	char* url = NULL;
	curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
	if (res != CURLE_OK) {
		nana::msgbox m(NULL, "Failed to check for updates");
		m << error;
		m();
		return true;
	}

	std::string remoteBuildNumber(url);
	remoteBuildNumber = remoteBuildNumber.substr(68);

	curl_easy_cleanup(curl);

	std::string localBuildNumber = std::to_string(BUILD_NUMBER);

	if (remoteBuildNumber != localBuildNumber) {
		nana::msgbox m(NULL, "Update Available", nana::msgbox::yes_no);
		m << "An update is available. You are currently on build #" << localBuildNumber << ", but build #" << remoteBuildNumber << " has been released. Do you wish to update now?";
		if (m() == m.pick_yes) {
			// We want to update
			{ // Download autoupdater
				CURL* curl = curl_easy_init();
				const std::string autoupdaterURL = RELEASES_URL + remoteBuildNumber + "/" + AUTOUPDATER_EXECUTABLE;
				curl_easy_setopt(curl, CURLOPT_URL, autoupdaterURL.c_str());
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curl, CURLOPT_MAXREDIRS, -1);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &FileWriteCallback);

				FILE* autoupdateFile = fopen(std::string(strPath + AUTOUPDATER_EXECUTABLE).c_str(), "wb");
				if (autoupdateFile) {
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, autoupdateFile);
					CURLcode res = curl_easy_perform(curl);

					char autoUpdaterError[CURL_ERROR_SIZE];
					curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, autoUpdaterError);
					autoUpdaterError[0] = 0;

					fclose(autoupdateFile);
					if (res != CURLE_OK) {
						nana::msgbox m(NULL, "Failed to download auto updater");
						m << autoUpdaterError;
						m();
						return true;
					}
				} else {
					nana::msgbox m(NULL, "Failed to download autoupdater");
					m << "Unable to open local file to write downloaded data to";
					m();
					curl_easy_cleanup(curl);
					return true;
				}

				curl_easy_cleanup(curl);
			}

			//std::string app_path(4096, '\0');
			//app_path.resize(GetModuleFileName(0, &app_path.front(), app_path.size()));
			//std::system(std::string("autoupdater.exe \"" + app_path + "\" https://github.com/littork/CEC-CSC160-Final-Project/releases/download/%23" + remoteBuildNumber + "/interface.exe").c_str());
			
			{ // Start the autoupdater
				STARTUPINFO si = {sizeof(STARTUPINFO)};
				si.cb = sizeof(si);
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_HIDE;
				PROCESS_INFORMATION pi;
				
				LPSTR cmdArgs = const_cast<LPSTR>(std::string("Autoupdater \"" + std::string(path) + "\" \"" + RELEASES_URL + remoteBuildNumber + "/" + INTERFACE_EXECUTABLE_REMOTE + "\"").c_str());

				std::string cmd = std::string(strPath + AUTOUPDATER_EXECUTABLE);
				CreateProcess(cmd.c_str(), cmdArgs, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
			}

			return false;
		}
	}

	return true;
}