#include "Macros.h"
#include <windows.h>

#include <string>
#include <regex>
#include <vector>

#include "UICore.h"

#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

#include "Autoupdate.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
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

	nana::label testLabel(form.form, nana::rectangle(200, 10, 100, 100));
	testLabel.format(true);
	testLabel.caption("<bold size=14>Test Label</>");
	testLabel.bgcolor(nana::color_rgb(COLOR_HEX_DEFAULT));

	// Test button

	std::vector<std::string> buttonTexts = {
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
	}

	// Shell

	Autoupdate::begin();

	// Show form

	form.display();

	UI::start();
	return 0;
}