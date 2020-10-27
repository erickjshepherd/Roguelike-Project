#include "GUI.h"
#include "SubMenus.h"

// global current menu
Menu* currentMenu_g;

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

int openMenu() {
	currentMenu_g = new mainMenu();
	int key;
	int selection = 0;
	int quit = 0;
	bool selecting = 1;
	Menu* prevMenu = NULL;

	while (selecting) {
		if (currentMenu_g != prevMenu) {
			currentMenu_g->drawMenu();
			currentMenu_g->drawArrow(selection);
			prevMenu = currentMenu_g;
		}
		key = handleEvents();
		if (key == EVENT_KEY_DOWN) {
			if (selection < ((currentMenu_g->numItems + currentMenu_g->numChangeable) - 1)) {
				selection++;
				currentMenu_g->drawArrow(selection);
			}
		}
		else if (key == EVENT_KEY_UP) {
			if (selection > 0) {
				selection--;
				currentMenu_g->drawArrow(selection);
			}
		}
		else if (key == EVENT_KEY_ENTER) {
			quit = currentMenu_g->selectItem(selection);
			if (quit != -1) {
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
