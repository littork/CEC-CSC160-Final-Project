#include "Macros.h"
#include <windows.h>

#include <string>
#include <regex>
#include <vector>

#include "UICore.h"

#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>

#include "Autoupdate.h"

class inline_widget : public nana::listbox::inline_notifier_interface {
private:
	virtual void create(nana::window wd) override {
		/*txt_.create(wd);
		txt_.events().click([this] {
			indicator_->selected(pos_);
		});

		txt_.events().mouse_move([this] {
			indicator_->hovered(pos_);
		});

		txt_.events().key_char([this](const nana::arg_keyboard& arg) {
			if (arg.key == nana::keyboard::enter) {
				arg.ignore = true;
				indicator_->modify(pos_, txt_.caption());
			}
		});

		txt_.events().text_changed([this]() {
			indicator_->modify(pos_, txt_.caption());
		});*/

		btn_.create(wd);
		btn_.caption("Launch");
		btn_.events().click([this] {
			auto& lsbox = dynamic_cast<nana::listbox&>(indicator_->host());
			lsbox.erase(lsbox.at(pos_));
		});

		btn_.events().mouse_move([this] {
			indicator_->hovered(pos_);
		});
	}

	virtual void activate(inline_indicator& ind, index_type pos) {
		indicator_ = &ind;
		pos_ = pos;
	}

	void notify_status(status_type status, bool status_on) override {
		if ((status_type::selecting == status) && status_on)
			txt_.focus();
	}

	void resize(const nana::size& dimension) override {
		auto sz = dimension;
		sz.width -= (sz.width < 50 ? 0 : 115); //Check for avoiding underflow.
		txt_.size(sz);

		nana::rectangle r(sz.width + 5, 0, 45, sz.height);
		btn_.move(r);
	}

	virtual void set(const value_type& value) {
		if (txt_.caption() != value)
			txt_.caption(value);
	}

	bool whether_to_draw() const override {
		return false;
	}
private:
	inline_indicator* indicator_ {nullptr};
	index_type pos_;
	nana::textbox txt_;
	nana::button btn_;
};

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

	nana::listbox gamesList(form.form, nana::rectangle{ 5, 5, 490, 290 });
	gamesList.append_header("Game Name");
	gamesList.append_header("Game Version");
	gamesList.append_header("Game Author(s)");
	gamesList.append_header("Game Launch");

	auto category = gamesList.at(0);
	gamesList.auto_draw(false);
	
	//nana::button launchButton = nana::button(form.form, nana::rectangle(10, 10, 100, 25));
	//launchButton.caption("Launch");

	// gamesList.at(0).append({ "OpenGL Test", "1.0", "Dylan Pozarnsky" });
	gamesList.at(0).append({ "Pacman", "1.0", "Alexander Marsh", "" });
	gamesList.at(0).append({ "Walking Simulator", "1.0", "Brendan Mccleery", "" });

	gamesList.at(0).inline_factory(3, nana::pat::make_factory<inline_widget>());

	gamesList.auto_draw(true);

	// Game details

	//nana::group gameDetailsGroup(form.form, nana::rectangle{ 10, 120, 480, 100 });

	//gameDetailsGroup.caption("Game Details");
	//gameDetailsGroup.bgcolor(NANA_COLOR_PURE_WHITE);

	/*nana::label gameNameLabel(gameDetailsGroup);
	gameNameLabel.caption("Game Name");

	gameDetailsGroup["gamename"] << gameNameLabel;

	gameDetailsGroup.collocate();*/

	// Show form

	form.display();

	UI::start();

	return 0;
}
