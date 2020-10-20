#pragma once

void renderGUI(int x, int y, int w, int h, int version, int type);
void drawTextBox(int x, int y, int w, int h, int type);
void drawBackground(int view);
void clearRect(SDL_Rect rect);
void drawMenu(int* playX, int* playY, int* optX, int* optY);
void openMenu();
