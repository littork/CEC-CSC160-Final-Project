#include "Macros.h"
#include <windows.h>

#include <string>
#include <regex>
#include <vector>

#include "UICore.h"

#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "Autoupdate.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	// Autoupdater

	if (!Autoupdate::check()) {
		// Catch auto update
		return 0;
	}

	// Form

	Form form = Form(500, 300);

	// Form title

	std::string dateTime = " (" BUILD_DATE ")";
	std::regex e("  ");

	std::string processedDateTime = "";
	std::regex_replace(std::back_inserter(processedDateTime), dateTime.begin(), dateTime.end(), e, " $2");
#ifdef BUILD_NUMBER
	form.setTitle("CEC Final Project - Release Build " + std::to_string(BUILD_NUMBER) + processedDateTime);
#else
	form.setTitle("CEC Final Project - Dev Build" + processedDateTime);
#endif

	// Test label

	/*nana::label testLabel(form.form, nana::rectangle(200, 10, 100, 100));
	testLabel.format(true);
	testLabel.caption("<bold size=14>Test Label</>");
	testLabel.bgcolor(nana::color_rgb(COLOR_HEX_DEFAULT));*/

	// Test button

	/*std::vector<std::string> buttonTexts = {
		"Game 1",
		"Game 2",
		"Game 3",
		"Game 4",
		"Game 5",
		"Game 6",
	};

	std::vector<std::shared_ptr<nana::button>> buttons;

	for (int i = 0; i < buttonTexts.size(); i++) {
		std::shared_ptr<nana::button> button = std::make_shared<nana::button>(form.form, nana::rectangle(10, 10 + (30 * i), 100, 25));
		button->caption(buttonTexts[i]);

		buttons.push_back(button);
	}*/

	// List of games

	nana::listbox gamesList(form.form, nana::rectangle{ 10, 10, 480, 100 });
	gamesList.append_header("Game Name");
	gamesList.append_header("Game Version");
	gamesList.append_header("Game Author(s)");

	auto category = gamesList.at(0);
	gamesList.auto_draw(false);
	
	// gamesList.at(0).append({ "OpenGL Test", "1.0", "Dylan Pozarnsky" });
	gamesList.at(0).append({ "Pacman", "1.0", "Alexander Marsh" });
	gamesList.at(0).append({ "Walking Simulator", "1.0", "Brendan Mccleery" });

	gamesList.auto_draw(true);

	// Game details

	nana::group gameDetailsGroup(form.form, nana::rectangle{ 10, 120, 480, 100 });

	gameDetailsGroup.caption("Game Details");
	gameDetailsGroup.bgcolor(NANA_COLOR_PURE_WHITE);

	/*nana::label gameNameLabel(gameDetailsGroup);
	gameNameLabel.caption("Game Name");

	gameDetailsGroup["gamename"] << gameNameLabel;

	gameDetailsGroup.collocate();*/

	// Show form

	form.display();

	UI::start();

	return 0;
}
