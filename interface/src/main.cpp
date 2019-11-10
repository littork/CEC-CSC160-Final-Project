#include "Macros.h"
#include <windows.h>

#include <string>

#include "UICore.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	Form form = Form(500, 300);

#ifdef BUILD_NUMBER
	form.setTitle("CEC Final Project - Release Build " + std::to_string(BUILD_NUMBER));
#else
	form.setTitle("CEC Final Project - Dev Build");
#endif

	form.display();

	UI::start();
	return 0;
}