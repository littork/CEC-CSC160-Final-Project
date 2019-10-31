#include <nana/gui.hpp>

#include "Macros.h"
#include <windows.h>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	nana::form form;

	form.caption("CEC Final Project");

	form.show();

	nana::exec();
	return 0;
}