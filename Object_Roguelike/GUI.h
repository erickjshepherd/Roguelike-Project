#pragma once

#define TITLEFONT 3
#define ITEMFONT 2
#define ARROWSPRITE 37

enum menuEnums {
	MAIN,
	OPTIONS,
	CONTINUE
};

#define numMenuItems 3
enum mainMenuEnums {
	PLAY_M,
	OPTIONS_M,
	EXIT_M
};

void renderTextBox(int x, int y, int w, int h, int version, int type);
void drawTextBox(int x, int y, int w, int h, int type);
void drawBackground(int view);
void clearRect(SDL_Rect rect);
void drawMenu(int* arrowX, int* arrowY, int* arrowSize, int titleFont, int itemFont);
void renderGUI(int x, int y, int w, int h, int sprite);
int openMenu();
void drawArrow(int arrowX, int arrowY, int arrowSize, int position, bool clear);
