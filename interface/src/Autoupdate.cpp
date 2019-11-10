#include "Autoupdate.h"

#include "Shell.h"
#include "Macros.h"

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
		nana::msgbox m("Update Available");
		m << "An update is available. You are currently on build #" << buildNumber << ", but build " << currentBuild << " has been released.";
		m();
	}
#endif
}