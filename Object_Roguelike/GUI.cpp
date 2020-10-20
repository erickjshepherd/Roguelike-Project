#include "Texture.h"
#include "SDLFuncs.h"
#include "GUI.h"

#define BACKGROUNDTYPE WHITEB_G

// renders a text box sprite
void renderGUI(int x, int y, int w, int h, int version, int type) {

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

void drawTextBox(int x, int y, int w, int h, int type) {

	int tileSize = tileSize_g;
	// draw corners
	renderGUI(x, y, tileSize, tileSize, 1, type); // top left
	renderGUI(x + (w - tileSize), y, tileSize, tileSize, 3, type); // top right
	renderGUI(x, y + (h - tileSize), tileSize, tileSize, 9, type); // bottom left
	renderGUI(x + (w - tileSize), y + (h - tileSize), tileSize, tileSize, 11, type); // bottom right
	// draw sides
	renderGUI(x, y + tileSize, tileSize, h - (tileSize * 2), 5, type); // left
	renderGUI(x + tileSize, y, w - (tileSize * 2), tileSize, 2, type); // top
	renderGUI(x + tileSize, y + (h - tileSize), w - (tileSize * 2), tileSize, 10, type); // bottom
	renderGUI(x + (w - tileSize), y + tileSize, tileSize, h - (tileSize * 2), 7, type); // right
	// fill center
	renderGUI(x + tileSize, y + tileSize, w - (tileSize * 2), h - (tileSize * 2), 6, type); // center
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
	renderGUI(rect.x, rect.y, rect.w, rect.h, 6, BACKGROUNDTYPE); // center
}

void drawMenu(int* playX, int* playY, int* optX, int* optY) {
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);
	SDL_RenderSetViewport(renderer_g, NULL);

	// set the background
	drawTextBox(0, 0, screenW, screenH, BACKGROUNDTYPE);

	// draw the title
	Texture titleT;
	int w, h;
	std::string titleText = "Insert Title Here";
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
}

void openMenu() {
	int playX, playY, optX, optY;
	drawMenu(&playX, &playY, &optX, &optY);
	SDL_RenderPresent(renderer_g);
}
