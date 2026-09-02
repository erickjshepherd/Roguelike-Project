#pragma once
#include <vector>
#include <string>

struct changeable {
	std::vector<std::string> states;
	int currentState = 0;
	int x = 0;
	int y = 0;
};

class Menu {
public:
	int itemFont;
	int headerFont;
	int numItems;
	int numHeaders;
	int numChangeable;
	int backgroundType;
	int arrowX;
	int arrowY;
	int arrowSize;
	std::vector<std::string> headerNames;
	std::vector<std::string> itemNames;
	std::vector<Menu*> children;
	std::vector<changeable> changeableItems;
	Menu* parent;
	int selection;


	Menu();
	~Menu();
	virtual int selectItem();
	void drawMenu();
	void drawArrow();
	void freeChildren();
	void openSubMenu(Menu* sub);
	void incrementState(int changeableIndex);
};
