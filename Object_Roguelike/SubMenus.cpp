#include "SubMenus.h"
#include "GUI.h"

// main menu
#define numMainItems 3
std::string mainMenuStrings[numMainItems] = {
	"Play",
	"Options",
	"Exit"
};
enum mainMenuEnums {
	PLAY_MAIN,
	OPTIONS_MAIN,
	EXIT_MAIN
};
#define numMainHeaders 1
std::string mainMenuHStrings[numMainHeaders] = {
	"Untitled Roguelike",
};

mainMenu::mainMenu() {
	itemFont = 2;
	headerFont = 3;
	numItems = numMainItems;
	for (int x = 0; x < numItems; x++) {
		itemNames.push_back(mainMenuStrings[x]);
	}
	numHeaders = numMainHeaders;
	for (int x = 0; x < numHeaders; x++) {
		headerNames.push_back(mainMenuHStrings[x]);
	}
}

mainMenu::~mainMenu() {
	
}

int mainMenu::selectItem(int item) {
	freeChildren();
	if (item == PLAY_MAIN) {
		return 0;
	}
	else if (item == OPTIONS_MAIN) {
		Menu* options = new optionsMenu;
		options->parent = this;
		children.push_back(options);
		currentMenu_g = options;
		return -1;
	}
	else if (item == EXIT_MAIN) {
		return 1;
	}
}

// options menu
#define numOptionItems 2
std::string optionMenuStrings[numOptionItems] = {
	"FullScreen",
	"Back"
};
enum optionMenuEnums {
	FULLSCREEN_O,
	BACK_O
};
#define numOptionHeaders 1
std::string optionMenuHStrings[numOptionHeaders] = {
	"Options"
};
optionsMenu::optionsMenu() {
	itemFont = 2;
	headerFont = 3;
	numItems = numOptionItems;
	for (int x = 0; x < numItems; x++) {
		itemNames.push_back(optionMenuStrings[x]);
	}
	numHeaders = numOptionHeaders;
	for (int x = 0; x < numHeaders; x++) {
		headerNames.push_back(optionMenuHStrings[x]);
	}
}

optionsMenu::~optionsMenu() {

}

int optionsMenu::selectItem(int item) {
	freeChildren();
	if (item == FULLSCREEN_O) {

	}
	else if (item == BACK_O) {
		currentMenu_g = parent;
	}
	return -1;
}