#include "Macros.h"
#include <windows.h>

#include <string>
#include <regex>

#include "UICore.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	Form form = Form(500, 300);

	std::string dateTime = " (" BUILD_DATE ")";
	std::regex e("  ");

	std::string processedDateTime = "";
	std::regex_replace(std::back_inserter(processedDateTime), dateTime.begin(), dateTime.end(), e, " $2");
#ifdef BUILD_NUMBER
	form.setTitle("CEC Final Project - Release Build " + std::to_string(BUILD_NUMBER) + processedDateTime);
#else
	form.setTitle("CEC Final Project - Dev Build" + processedDateTime);
#endif

	form.display();

	UI::start();
	return 0;
}