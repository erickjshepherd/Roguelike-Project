#pragma once
#include <vector>

class Menu {
public:
	int itemFont;
	int headerFont;
	int numItems;
	int numHeaders;
	int backgroundType;
	int arrowX;
	int arrowY;
	int arrowSize;
	std::vector<std::string> headerNames;
	std::vector<std::string> itemNames;
	std::vector<Menu*> children;
	Menu* parent;


	Menu();
	~Menu();
	virtual int selectItem(int item);
	void drawMenu();
	void drawArrow(int position);
	void freeChildren();
	void openSubMenu(Menu* sub);
};
