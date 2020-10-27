#pragma once
#include "Menu.h"

class mainMenu : public Menu {
public:
	mainMenu();
	~mainMenu();
	int selectItem(int item);
};

class optionsMenu : public Menu {
public:
	optionsMenu();
	~optionsMenu();
	int selectItem(int item);
	void apply();
};
