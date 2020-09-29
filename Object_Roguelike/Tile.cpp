#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"
#include "Texture.h"
#include <iostream>
#include <conio.h>

Tile::Tile() {
	blocking = 1;
	border = 0;
	sprite = -1;
	icon = ' ';
	under = nullptr;
}

Tile::Tile(char iconI, bool blockingI){
	blocking = blockingI;
	border = 0;
	sprite = -1;
	icon = iconI;
	under = nullptr;
}

Tile::~Tile(){
	try {
		delete under;
	}
	catch(int e){
	
	}
}

void Tile::Player_Interact() {

}

void Tile::Player_Step() {
	global_map->player->drawUnderInfo(); // todo: handle more of this here instead of in player class
}

void Tile::Spell_Interact(int damage, int effect, int intensity) {

}

int Tile::Player_Attack(int damage) {
	return 0;
}

void Tile::render(int x, int y, int colorIn) {

	// set the view port
	SDL_RenderSetViewport(renderer_g, &mapView_g);

	// render the under tile with the same color
	// todo: add input for only applying color to top layer
	if (under != nullptr) {
		under->render(x, y, colorIn);
	}

	// -1 is current color
	if (colorIn == -1) {
		colorIn = color;
	}
	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[spritePath * NUMCOLORS + colorIn];

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	int typeXOffset = spriteType % spriteSheetW;
	int typeYOffset = spriteType / spriteSheetW;
	clipX += typeXOffset * 16;
	clipY += typeYOffset * 16;

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = 16;
	clip->h = 16;

	spriteSheet->render(x, y, clip);

	delete clip;
}

void Tile::flash(int colorIn, int delay) {
	int x, y;
	onScreen(&x, &y);
	x *= TILE_SIZE;
	y *= TILE_SIZE;
	if (x == -1) {
		return;
	}
	if (under != nullptr) {
		under->render(x, y, -1);
	}

	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[spritePath * NUMCOLORS + colorIn];

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	int typeXOffset = spriteType % spriteSheetW;
	int typeYOffset = spriteType / spriteSheetW;
	clipX += typeXOffset * 16;
	clipY += typeYOffset * 16;

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = 16;
	clip->h = 16;

	SDL_RenderSetViewport(renderer_g, &mapView_g);
	spriteSheet->render(x, y, clip);
	SDL_RenderPresent(renderer_g);
	Sleep(delay);
	render(x, y, color);
	SDL_RenderPresent(renderer_g);

	delete clip;
}

// returns coordinates if the enemy is on the screen
void Tile::onScreen(int* X, int* Y) {

	int view_start = global_map->player->view_start;
	int view_distance = global_map->player->view_distance;
	int size = global_map->size;
	int x, y, xy;

	// get the width of the square to draw
	int view_size = (view_distance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = view_start;
			xy += y * size;
			xy += x;

			if (xy >= 0 && xy < (size * size)) {

				if (global_map->map[xy] == this) {

					*X = x;
					*Y = y;
					return;
				}
			}
		}
	}

	*X = -1;
	*Y = -1;
	return;
}

void Tile::drawUnderInfo() {
	global_map->player->clearStats(INSPECTINFO);

	if (under->description.length() <= 0) {
		return;
	}

	// set the view port
	SDL_RenderSetViewport(renderer_g, &statsView_g);

	// todo: move to draw_stats
	std::string infoStr;
	infoStr.append(under->description);
	Texture text;
	text.loadFromRenderedText(infoStr, textColor_g);
	text.render(0, INSPECTINFO * TEXTSPACE, NULL);
}
