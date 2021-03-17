#pragma once
#include "Texture.h"
#include "Menu.h"

#define ARROWSPRITE 37
#define BACKGROUNDTYPE WHITEB_G

extern Menu* currentMenu_g;

enum infoWindows {
	EVENTS_W,
	UNDER_W,
	STATS_W,
	WEAPON_W,
	SPELL_W
};

void renderTextBox(int x, int y, int w, int h, int version, int type);
void drawTextBox(int x, int y, int w, int h, int type);
void drawBackground(int view);
void clearRect(SDL_Rect rect);
void clearInfoWindow();
void renderGUI(int x, int y, int w, int h, int sprite);
int openMenu(Menu* menu);
