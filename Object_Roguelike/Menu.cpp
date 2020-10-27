#include "Menu.h"
#include "GUI.h"
#include "Texture.h"

Menu::Menu() {
	itemFont = 2;
	headerFont = 3;
	numItems = 0;
	arrowX = 0;
	arrowY = 0;
	arrowSize = 0;
}
Menu::~Menu() {
	
}

int Menu::selectItem(int item) {
	return 0;
}

void Menu::drawMenu() {
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);
	SDL_RenderSetViewport(renderer_g, NULL);

	// set the background
	drawTextBox(0, 0, screenW, screenH, BACKGROUNDTYPE);

	// draw the headers
	Texture titleT;
	int w, h;
	for (int x = 0; x < numHeaders; x++) {
		Texture texture;
		std::string headerText = headerNames[x];
		TTF_SizeText(fonts_g[headerFont], headerText.c_str(), &w, &h);
		texture.loadFromRenderedText(headerText, textColor_g, headerFont);
		texture.render((screenW - w) / 2, h, NULL);
	}

	// draw the rest of the menu items
	arrowY = h * ((numHeaders * 2) + 1);
	int currentY = arrowY;
	int maxX = 0;
	for (int x = 0; x < numItems; x++) {
		Texture texture;
		std::string itemText = itemNames[x];
		TTF_SizeText(fonts_g[itemFont], itemText.c_str(), &w, &h);
		texture.loadFromRenderedText(itemText, textColor_g, itemFont);
		texture.render((screenW - w) / 2, currentY, NULL);
		currentY += h * 2;
		if (w > maxX) {
			maxX = w;
			arrowX = (screenW - w) / 2;
		}
	}

	// draw the changeable items
	for (int x = 0; x < numChangeable; x++) {
		Texture texture;
		int currentState = changeableItems[x].currentState;
		std::string itemText = changeableItems[x].states[currentState];
		TTF_SizeText(fonts_g[itemFont], itemText.c_str(), &w, &h);
		texture.loadFromRenderedText(itemText, textColor_g, itemFont);
		texture.render((screenW - w) / 2, currentY, NULL);
		changeableItems[x].x = (screenW - w) / 2;
		changeableItems[x].y = currentY;
		currentY += h * 2;
		if (w > maxX) {
			maxX = w;
			arrowX = (screenW - w) / 2;
		}
	}

	// return the initial arrow position
	int fontH = TTF_FontHeight(fonts_g[itemFont]);
	arrowSize = fontH;
	bool round = (arrowSize % 16) > 0;
	arrowSize /= 16;
	if (round) {
		(arrowSize)++;
	}
	arrowSize *= 16;
	arrowX = arrowX - (arrowSize * 2);
	arrowY = arrowY - (arrowSize - fontH) / 2;
}

void Menu::drawArrow(int position) {
	// set up the arrow
	int fontH = TTF_FontHeight(fonts_g[itemFont]);
	for (int x = 0; x < numItems + numChangeable; x++) {
		int offset = fontH * 2 * x;
		int newArrowY = arrowY + offset;
		if (x != position) {
			SDL_Rect clear = { arrowX, newArrowY, arrowSize, arrowSize };
			clearRect(clear);
		}
		else {
			renderGUI(arrowX, newArrowY, arrowSize, arrowSize, ARROWSPRITE);
		}
	}
	SDL_RenderPresent(renderer_g);
}

void Menu::freeChildren() {
	while (!children.empty()) {
		delete children.back();
		children.pop_back();
	}
}

void Menu::openSubMenu(Menu* sub) {
	currentMenu_g = sub;
	children.push_back(sub);
}
