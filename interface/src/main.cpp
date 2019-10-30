#include <nana/gui.hpp>

#include "Macros.h"

int WinMain() {
	nana::form form;

	form.caption("CEC Final Project");

	form.show();

	nana::exec();
	return 0;
}