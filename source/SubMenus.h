#pragma once
#include "Menu.h"

class mainMenu : public Menu {
public:
	mainMenu();
	~mainMenu();
	int selectItem();
};

class optionsMenu : public Menu {
public:
	optionsMenu();
	~optionsMenu();
	int selectItem();
	void apply();
};

extern optionsMenu* optionsMenu_g;

class pauseMenu : public Menu {
public:
	pauseMenu();
	~pauseMenu();
	int selectItem();
};

void initMenus();

void freeMenus();
