#include "Autoupdate.h"

#include "Shell.h"
#include "Macros.h"

#include <Windows.h>

#include <nana/gui.hpp>

std::string Autoupdate::buildNumber = "";

void Autoupdate::begin() {
#ifdef BUILD_NUMBER
	if (!buildNumber.size()) {
		buildNumber = std::to_string(BUILD_NUMBER);
	}

	std::string currentBuild = Shell::run("curl -fsSLI -o /dev/null -w %{url_effective} " REPOSITORY_URL);
	currentBuild = currentBuild.substr(68);

	if (buildNumber != currentBuild) {
		nana::msgbox m(NULL, "Update Available", nana::msgbox::yes_no);
		m << "An update is available. You are currently on build #" << buildNumber << ", but build #" << currentBuild << " has been released. Do you wish to update now?";
		if (m() == m.pick_yes) {
			// We want to update
			std::system("curl -L https://github.com/littork/CEC-CSC160-Final-Project/raw/master/interface/autoupdater.exe -o autoupdater.exe");
			std::string app_path(4096, '\0');
			app_path.resize(GetModuleFileName(0, &app_path.front(), app_path.size()));
			std::system(std::string("autoupdater.exe \"" + app_path + "\" https://github.com/littork/CEC-CSC160-Final-Project/releases/download/%23" + buildNumber + "/interface_x64.exe").c_str());
		}
	}
#endif
}