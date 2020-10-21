#include "Texture.h"
#include "SDLFuncs.h"
#include "GUI.h"

#define BACKGROUNDTYPE WHITEB_G

// renders a text box sprite
void renderTextBox(int x, int y, int w, int h, int version, int type) {

	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[GUIPATH * NUMCOLORS];

	// get the location on the sheet
	int clipX = 0;
	int clipY = 112;
	// text box sprites are in rows of 4
	int xOffset = version % 4;
	int yOffset = version / 4;
	// wall sets are in columns of 4
	int typeXOffset = type / 4;
	int typeYOffset = type % 4;
	clipX += (xOffset * TILE_SOURCE_SIZE) + (typeXOffset * TILE_SOURCE_SIZE * 4);
	clipY += (yOffset * TILE_SOURCE_SIZE) + (typeYOffset * TILE_SOURCE_SIZE * 4);

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = TILE_SOURCE_SIZE;
	clip->h = TILE_SOURCE_SIZE;

	// render the tile
	SDL_Rect renderQuad = { x, y, w, h };
	SDL_RenderCopy(renderer_g, spriteSheet->getTexture(), clip, &renderQuad);

	delete clip;
}

void renderGUI(int x, int y, int w, int h, int sprite) {
	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[GUIPATH * NUMCOLORS];

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	// text box sprites are in rows of 4
	int xOffset = sprite % 16;
	int yOffset = sprite / 16;
	clipX += (xOffset * TILE_SOURCE_SIZE);
	clipY += (yOffset * TILE_SOURCE_SIZE);

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = TILE_SOURCE_SIZE;
	clip->h = TILE_SOURCE_SIZE;

	// render the tile
	SDL_Rect renderQuad = { x, y, w, h };
	SDL_RenderCopy(renderer_g, spriteSheet->getTexture(), clip, &renderQuad);

	delete clip;
}

void drawTextBox(int x, int y, int w, int h, int type) {

	int tileSize = tileSize_g;
	// draw corners
	renderTextBox(x, y, tileSize, tileSize, 1, type); // top left
	renderTextBox(x + (w - tileSize), y, tileSize, tileSize, 3, type); // top right
	renderTextBox(x, y + (h - tileSize), tileSize, tileSize, 9, type); // bottom left
	renderTextBox(x + (w - tileSize), y + (h - tileSize), tileSize, tileSize, 11, type); // bottom right
	// draw sides
	renderTextBox(x, y + tileSize, tileSize, h - (tileSize * 2), 5, type); // left
	renderTextBox(x + tileSize, y, w - (tileSize * 2), tileSize, 2, type); // top
	renderTextBox(x + tileSize, y + (h - tileSize), w - (tileSize * 2), tileSize, 10, type); // bottom
	renderTextBox(x + (w - tileSize), y + tileSize, tileSize, h - (tileSize * 2), 7, type); // right
	// fill center
	renderTextBox(x + tileSize, y + tileSize, w - (tileSize * 2), h - (tileSize * 2), 6, type); // center
}

void drawBackground(int view) {
	SDL_RenderSetViewport(renderer_g, NULL);

	// get the screen dimensions
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);

	// draw map background
	if (view == 0 || view == -1) {
		drawTextBox(0, 0, screenH, screenH, BACKGROUNDTYPE);
	}
	// draw stat background
	if (view == 1 || view == -1) {
		drawTextBox(statsView_g.x - (tileSize_g / 2), statsView_g.y - (tileSize_g / 2), statsView_g.w + tileSize_g, statsView_g.h + tileSize_g, BACKGROUNDTYPE);
	}
	// draw event background
	if (view == 2 || view == -1) {
		drawTextBox(eventsView_g.x - (tileSize_g / 2), eventsView_g.y - (tileSize_g / 2), eventsView_g.w + tileSize_g, eventsView_g.h + tileSize_g, BACKGROUNDTYPE);
	}
}

void clearRect(SDL_Rect rect) {
	renderTextBox(rect.x, rect.y, rect.w, rect.h, 6, BACKGROUNDTYPE); // center
}

void drawMenu(int* playX, int* playY, int* optX, int* optY, int* exitX, int* exitY) {
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);
	SDL_RenderSetViewport(renderer_g, NULL);

	// set the background
	drawTextBox(0, 0, screenW, screenH, BACKGROUNDTYPE);

	// draw the title
	Texture titleT;
	int w, h;
	std::string titleText = "Untitled Roguelike";
	TTF_SizeText(fonts_g[3], titleText.c_str(), &w, &h);
	titleT.loadFromRenderedText(titleText, textColor_g, 3);
	int titleX = (screenW - w) / 2;
	titleT.render(titleX, h, NULL);

	// draw the play button
	Texture playT;
	std::string playText = "Play";
	*playY = h * 3;
	TTF_SizeText(fonts_g[2], playText.c_str(), &w, &h);
	playT.loadFromRenderedText(playText, textColor_g, 2);
	*playX = (screenW - w) / 2;
	playT.render(*playX, *playY, NULL);

	// draw the options button
	Texture optionsT;
	std::string optionsText = "Options";
	TTF_SizeText(fonts_g[2], optionsText.c_str(), &w, &h);
	optionsT.loadFromRenderedText(optionsText, textColor_g, 2);
	*optY = *playY + (h * 2);
	*optX = (screenW - w) / 2;
	optionsT.render(*optX, *optY, NULL);

	// draw the exit button
	Texture exitT;
	std::string exitText = "Exit";
	TTF_SizeText(fonts_g[2], exitText.c_str(), &w, &h);
	exitT.loadFromRenderedText(exitText, textColor_g, 2);
	*exitY = *optY + (h * 2);
	*exitX = (screenW - w) / 2;
	exitT.render(*exitX, *exitY, NULL);
}

// todo: update this to handle sub-menus. ex: options sub-menu
int openMenu() {
	// draw the menu items
	int playX, playY, optX, optY, exitX, exitY;
	drawMenu(&playX, &playY, &optX, &optY, &exitX, &exitY);

	// set up the arrow
	int fontH = TTF_FontHeight(fonts_g[2]);
	int arrowSize = fontH;
	bool round = (arrowSize % 16) > 0;
	arrowSize /= 16;
	if (round) {
		arrowSize++;
	}
	arrowSize *= 16;
	int arrowX = optX - (arrowSize * 2);
	int arrowY = playY;
	arrowX -= (arrowSize - fontH) / 2;
	renderGUI(arrowX, arrowY, arrowSize, arrowSize, 37);

	SDL_RenderPresent(renderer_g);
	int key;
	int selection = 0;
	int quit = 0;
	bool selecting = 1;
	while (selecting) {
		key = handleEvents();
		if (key == EVENT_KEY_DOWN) {
			if (selection < 2) {
				selection++;
				SDL_Rect clear = { arrowX, arrowY, arrowSize, arrowSize };
				clearRect(clear);
				arrowY += fontH * 2;
				renderGUI(arrowX, arrowY, arrowSize, arrowSize, 37);
				SDL_RenderPresent(renderer_g);
			}
		}
		else if (key == EVENT_KEY_UP) {
			if (selection > 0) {
				selection--;
				SDL_Rect clear = { arrowX, arrowY, arrowSize, arrowSize };
				clearRect(clear);
				arrowY -= fontH * 2;
				renderGUI(arrowX, arrowY, arrowSize, arrowSize, 37);
				SDL_RenderPresent(renderer_g);
			}
		}
		else if (key == EVENT_KEY_ENTER) {
			if (selection == 0) {
				selecting = 0;
			}
			else if (selection == 2) {
				quit = 1;
				selecting = 0;
			}
		}
		else if (key == EVENT_QUIT || key == EVENT_KEY_ESC) {
			quit = 1;
			selecting = 0;
		}
	}
	return quit;
}
