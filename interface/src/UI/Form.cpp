#include "Form.h"

#include <Macros.h>

#include <Windows.h>

Form::Form(const unsigned int& width, const unsigned int& height) :
	form(nana::form(
		nana::API::make_center(width, height),
		nana::form::appear::decorate<nana::form::appear::minimize>()
	)),
	content(
		nana::panel<true>(form, {0, 0, width, height})
	)
{
	content.bgcolor(nana::color_rgb(COLOR_HEX_DEFAULT));

	std::wstring app_path(4096, '\0');
	app_path.resize(GetModuleFileNameW(0, &app_path.front(), app_path.size()));
	form.icon(nana::paint::image(app_path));
}

void Form::setTitle(const std::string& title) {
	form.caption(title);
}

void Form::display() {
	form.show();
}