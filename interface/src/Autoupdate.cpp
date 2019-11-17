#include "Autoupdate.h"

#include "Macros.h"

#include <Windows.h>

#include <nana/gui.hpp>
#include <curl/curl.h>

std::string Autoupdate::buildNumber = "";

#ifndef BUILD_NUMBER
#define BUILD_NUMBER 0
#endif

size_t WriteCallback(char* buf, size_t size, size_t nmemb, void* up) {
	return size * nmemb;
}

bool Autoupdate::check() {
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, REPOSITORY_URL);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, -1);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
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
	}

	std::string remoteBuildNumber(url);
	remoteBuildNumber = remoteBuildNumber.substr(68);

	curl_easy_cleanup(curl);

	std::string localBuildNumber = std::to_string(BUILD_NUMBER);

	if (remoteBuildNumber != localBuildNumber) {
		nana::msgbox m(NULL, "Update Available", nana::msgbox::yes_no);
		m << "An update is available. You are currently on build #" << localBuildNumber << ", but build #" << remoteBuildNumber << " has been released. Do you wish to update now?";
		if (m() == m.pick_yes) {
			return false;
			// We want to update
			//std::system("curl -L https://github.com/littork/CEC-CSC160-Final-Project/raw/master/interface/autoupdater.exe -o autoupdater.exe");
			//std::string app_path(4096, '\0');
			//app_path.resize(GetModuleFileName(0, &app_path.front(), app_path.size()));
			//std::system(std::string("autoupdater.exe \"" + app_path + "\" https://github.com/littork/CEC-CSC160-Final-Project/releases/download/%23" + remoteBuildNumber + "/interface_x64.exe").c_str());
		}
	}

	return true;
}