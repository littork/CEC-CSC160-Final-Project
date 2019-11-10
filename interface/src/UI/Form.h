#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>

#include <string>

class Form {
public:
	Form(const unsigned int& width, const unsigned int& height);
private:
	nana::form form;

	nana::panel<true> content;
public:
	void setTitle(const std::string& title);
public:
	void display();
};