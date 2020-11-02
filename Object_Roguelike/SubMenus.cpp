#include "SubMenus.h"
#include "GUI.h"

// global menus for saving states
optionsMenu* optionsMenu_g;

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
	selection = 0;
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

int mainMenu::selectItem() {
	if (selection == PLAY_MAIN) {
		return 0;
	}
	else if (selection == OPTIONS_MAIN) {
		openSubMenu(optionsMenu_g);
		return -1;
	}
	else if (selection == EXIT_MAIN) {
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
	selection = 0;
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

int optionsMenu::selectItem() {
	int changeableIndex = 0;
	if (selection > numOptionItems) {
		changeableIndex = (selection - numItems) + changeableItems.size();
	}
	if (selection == BACK_O) {
		currentMenu_g = parent;
	}
	else if (selection == FULLSCREEN_O) {
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
	else if (selection == APPLY_O) {
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
				drawArrow();
				SDL_RenderPresent(renderer_g);
			}
			else {
				SDL_SetWindowFullscreen(window_g, SDL_WINDOW_FULLSCREEN_DESKTOP);
				rendererInit();
				drawMenu();
				drawArrow();
				SDL_RenderPresent(renderer_g);
			}
		}
	}
}

// pause menu
#define numPauseItems 3
std::string pauseMenuStrings[numPauseItems] = {
	"Resume",
	"Options",
	"Exit"
};
enum pauseMenuEnums {
	RESUME_PAUSE,
	OPTIONS_PAUSE,
	EXIT_PAUSE
};
#define numPauseHeaders 1
std::string pauseMenuHStrings[numPauseHeaders] = {
	"Untitled Roguelike",
};

pauseMenu::pauseMenu() {
	itemFont = 2;
	headerFont = 3;
	selection = 0;
	numItems = numPauseItems;
	for (int x = 0; x < numItems; x++) {
		itemNames.push_back(pauseMenuStrings[x]);
	}
	numHeaders = numPauseHeaders;
	for (int x = 0; x < numHeaders; x++) {
		headerNames.push_back(pauseMenuHStrings[x]);
	}
}

pauseMenu::~pauseMenu() {

}

int pauseMenu::selectItem() {
	if (selection == RESUME_PAUSE) {
		return 0;
	}
	else if (selection == OPTIONS_PAUSE) {
		openSubMenu(optionsMenu_g);
		return -1;
	}
	else if (selection == EXIT_PAUSE) {
		return 1;
	}
}

void initMenus() {
	optionsMenu_g = new optionsMenu();
}

void freeMenus() {
	delete optionsMenu_g;
}
