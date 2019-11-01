#include <nana/gui.hpp>

#include "Macros.h"
#include <windows.h>

#include <string>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	nana::form form;

#ifdef BUILD_NUMBER
	form.caption("CEC Final Project - Build " + std::to_string(BUILD_NUMBER));
#else
	form.caption("CEC Final Project - Dev Build");
#endif

	form.show();

	nana::exec();
	return 0;
}