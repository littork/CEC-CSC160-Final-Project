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

#include <curl/curl.h>

#include "Autoupdate.h"

struct Game {
	Game(const std::string& _name, const std::string& _version, const std::string& _author, const std::string& _filename, const bool& _incompatible) : name(_name), version(_version), author(_author), filename(_filename), incompatible(_incompatible) {}

public:
	std::string name;
	std::string version;
	std::string author;
	std::string filename;
	bool incompatible;
};

const std::vector<Game> games {
	Game("Walking Simulator", "1.0", "Brendan Mccleery", "walkingsimulator.exe", false),
	Game("Pacman", "1.0", "Alexander Marsh", "pacman.exe", true),
	Game("CEC Mancala", "1.0", "Dylan Pozarnsky", "cecmancala.exe", false)
};

size_t FileWriteCallbackMain(char* buf, size_t size, size_t nmemb, void* up) {
	size_t written = fwrite(buf, size, nmemb, ( FILE*) up);
	return written;
}

void launchGame(const int& gameIndex) {
	const auto targetGame = games[gameIndex];

	if (games[gameIndex].incompatible) {
		nana::msgbox m(NULL, "Error");
		m << "Unfortunately, your system architecture does not meet the minimum requirements needed to run this project";
		m();

		return;
	}

	const auto stringPath = Autoupdate::stringPath();
	const auto gameExecutable = targetGame.filename;

	const auto remoteBuildNumber = Autoupdate::getRemoteBuildNumber();

	// Erase old game executable if it exists
	Autoupdate::cleanup(stringPath, gameExecutable);

	{ // Download game executable
		CURL* curl = curl_easy_init();
		const std::string autoupdaterURL = RELEASES_URL + remoteBuildNumber + "/" + gameExecutable;
		curl_easy_setopt(curl, CURLOPT_URL, autoupdaterURL.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, -1);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &FileWriteCallbackMain);

		FILE* autoupdateFile = fopen(std::string(stringPath + gameExecutable).c_str(), "wb");
		if (autoupdateFile) {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, autoupdateFile);
			CURLcode res = curl_easy_perform(curl);

			char autoUpdaterError[CURL_ERROR_SIZE];
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, autoUpdaterError);
			autoUpdaterError[0] = 0;

			fclose(autoupdateFile);
			if (res != CURLE_OK) {
				nana::msgbox m(NULL, "Failed to download game: " + targetGame.name);
				m << autoUpdaterError;
				m();

				curl_easy_cleanup(curl);

				return;
			}
		} else {
			nana::msgbox m(NULL, "Failed to download game: " + targetGame.name);
			m << "Unable to open local file to write downloaded data to";
			m();

			curl_easy_cleanup(curl);

			return;
		}

		curl_easy_cleanup(curl);
	}

	{ // Start the game
		STARTUPINFO si = {sizeof(STARTUPINFO)};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		PROCESS_INFORMATION pi;

		std::string cmd = std::string(stringPath + gameExecutable);
		CreateProcess(cmd.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}
}

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
			launchGame(pos_.item);
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

	for (int i = 0; i < games.size(); i++) {
		gamesList.at(0).append({games[i].name, games[i].version, games[i].author, ""});
	}

	gamesList.at(0).inline_factory(3, nana::pat::make_factory<inline_widget>());

	gamesList.auto_draw(true);

	// Show form

	form.display();

	UI::start();

	return 0;
}
