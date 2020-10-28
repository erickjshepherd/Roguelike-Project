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
#define numOptionItems 1
std::string optionMenuStrings[numOptionItems] = {
	"Back"
};
enum optionMenuEnums {
	BACK_O,
	FULLSCREEN_O,
	APPLY_O
};
#define numOptionHeaders 1
std::string optionMenuHStrings[numOptionHeaders] = {
	"Options"
};
#define offOnStates 2
std::string offOn[offOnStates] = {
	"Off",
	"On"
};
enum offOnEnums {
	OFF_STATE,
	ON_STATE
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

	// set up the options
	// fullscreen
	changeable fullscreen;
	fullscreen.currentState = 0;
	for (int x = 0; x < offOnStates; x++) {
		fullscreen.states.push_back("Fullscreen: " + offOn[x]);
	}
	changeableItems.push_back(fullscreen);
	// apply
	changeable apply;
	apply.currentState = 0;
	apply.states.push_back("Apply");
	changeableItems.push_back(apply);

	numChangeable = changeableItems.size();
}

optionsMenu::~optionsMenu() {

}

int optionsMenu::selectItem(int item) {
	freeChildren();
	int changeableIndex = 0;
	if (item > numOptionItems) {
		changeableIndex = (item - numItems) + changeableItems.size();
	}
	if (item == BACK_O) {
		currentMenu_g = parent;
	}
	else if (item == FULLSCREEN_O) {
		// todo: make an update state function out of this
		int w, h;
		int currentState = changeableItems[changeableIndex].currentState;
		TTF_SizeText(fonts_g[itemFont], changeableItems[changeableIndex].states[currentState].c_str(), &w, &h);
		SDL_Rect clear = {changeableItems[changeableIndex].x, changeableItems[changeableIndex].y, w, h};
		changeableItems[changeableIndex].currentState++;
		if (changeableItems[changeableIndex].currentState >= changeableItems[changeableIndex].states.size()) {
			changeableItems[changeableIndex].currentState = 0;
		}
		currentState = changeableItems[changeableIndex].currentState;
		clearRect(clear);
		Texture texture;
		texture.loadFromRenderedText(changeableItems[changeableIndex].states[currentState], textColor_g, itemFont);
		texture.render(changeableItems[changeableIndex].x, changeableItems[changeableIndex].y, NULL);
		SDL_RenderPresent(renderer_g);
	}
	else if (item == APPLY_O) {
		apply();
	}
	return -1;
}

void optionsMenu::apply() {
	for (int x = 0; x < numChangeable; x++) {
		int currentItem = numItems + x;
		if (currentItem == FULLSCREEN_O) {
			if (changeableItems[x].currentState == OFF_STATE) {
				SDL_SetWindowFullscreen(window_g, 0);
				rendererInit();
				drawMenu();
				drawArrow(currentItem);
				SDL_RenderPresent(renderer_g);
			}
			else {
				SDL_SetWindowFullscreen(window_g, SDL_WINDOW_FULLSCREEN_DESKTOP);
				rendererInit();
				drawMenu();
				drawArrow(currentItem);
				SDL_RenderPresent(renderer_g);
			}
		}
	}
}
