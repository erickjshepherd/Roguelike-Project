#pragma once
#include "Texture.h"
#include "Menu.h"

#define ARROWSPRITE 37
#define BACKGROUNDTYPE WHITEB_G

extern Menu* currentMenu_g;

void renderTextBox(int x, int y, int w, int h, int version, int type);
void drawTextBox(int x, int y, int w, int h, int type);
void drawBackground(int view);
void clearRect(SDL_Rect rect);
void renderGUI(int x, int y, int w, int h, int sprite);
int openMenu();
