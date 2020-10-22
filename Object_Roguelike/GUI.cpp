#include "Texture.h"
#include "SDLFuncs.h"
#include "GUI.h"

#define BACKGROUNDTYPE WHITEB_G

std::string mainMenuStrings[numMenuItems] = {
	"Play",
	"Options",
	"Exit"
};

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

void drawMenu(int* arrowX, int* arrowY, int* arrowSize, int titleFont, int itemFont) {
	int screenW, screenH;
	SDL_GetWindowSize(window_g, &screenW, &screenH);
	SDL_RenderSetViewport(renderer_g, NULL);
	
	// set the background
	drawTextBox(0, 0, screenW, screenH, BACKGROUNDTYPE);

	// draw the title
	Texture titleT;
	int w, h;
	std::string titleText = "Untitled Roguelike";
	TTF_SizeText(fonts_g[titleFont], titleText.c_str(), &w, &h);
	titleT.loadFromRenderedText(titleText, textColor_g, 3);
	int titleX = (screenW - w) / 2;
	titleT.render(titleX, h, NULL);

	// draw the rest of the menu items
	*arrowY = h * 3;
	int currentY = *arrowY;
	int maxX = 0;
	for (int x = 0; x < numMenuItems; x++) {
		Texture texture;
		std::string itemText = mainMenuStrings[x];
		TTF_SizeText(fonts_g[itemFont], itemText.c_str(), &w, &h);
		texture.loadFromRenderedText(itemText, textColor_g, itemFont);
		texture.render((screenW - w) / 2, currentY, NULL);
		currentY += h * 2;
		if (w > maxX) {
			maxX = w;
			*arrowX = (screenW - w) / 2;
		}
	}

	// return the initial arrow position
	int fontH = TTF_FontHeight(fonts_g[ITEMFONT]);
	*arrowSize = fontH;
	bool round = (*arrowSize % 16) > 0;
	*arrowSize /= 16;
	if (round) {
		(*arrowSize)++;
	}
	*arrowSize *= 16;
	*arrowX = *arrowX - (*arrowSize * 2);
	*arrowY = *arrowY - (*arrowSize - fontH) / 2;
}

void drawArrow(int arrowX, int arrowY, int arrowSize, int position, bool clear) {
	// set up the arrow
	int fontH = TTF_FontHeight(fonts_g[ITEMFONT]);
	int offset = fontH * 2 * position;
	arrowY += offset;
	if (clear) {
		SDL_Rect clear = { arrowX, arrowY, arrowSize, arrowSize };
		clearRect(clear);
	}
	else {
		renderGUI(arrowX, arrowY, arrowSize, arrowSize, ARROWSPRITE);
	}
	SDL_RenderPresent(renderer_g);
}

// todo: create a menu class instead
int openMenu() {
	// draw the menu items
	int arrowX, arrowY, arrowSize;
	drawMenu(&arrowX, &arrowY, &arrowSize, TITLEFONT, ITEMFONT);
	drawArrow(arrowX, arrowY, arrowSize, 0, 0);

	int key;
	int selection = 0;
	int quit = 0;
	bool selecting = 1;
	int menu = MAIN;
	int numItems = numMenuItems;
	while (selecting) {
		key = handleEvents();
		if (key == EVENT_KEY_DOWN) {
			if (selection < (numItems - 1)) {
				drawArrow(arrowX, arrowY, arrowSize, selection, 1);
				selection++;
				drawArrow(arrowX, arrowY, arrowSize, selection, 0);
			}
		}
		else if (key == EVENT_KEY_UP) {
			if (selection > 0) {
				drawArrow(arrowX, arrowY, arrowSize, selection, 1);
				selection--;
				drawArrow(arrowX, arrowY, arrowSize, selection, 0);
			}
		}
		else if (key == EVENT_KEY_ENTER) {
			if (selection == PLAY_M && menu == MAIN) {
				return 0;
			}
			else if (selection == EXIT_M && menu == MAIN) {
				return 1;
			}
		}
		else if (key == EVENT_QUIT || key == EVENT_KEY_ESC) {
			quit = 1;
			selecting = 0;
		}
	}
	return quit;
}
