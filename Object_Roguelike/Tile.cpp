#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"
#include "Texture.h"
#include "Console.h"
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

void Tile::drawUnderInfo() {
	short view_size = (short)(global_map->player->view_distance * 2) + 1;
	short x = 2 * view_size;

	global_map->player->clearStats(INSPECTINFO);

	COORD position = { x, 14 + 1 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);;
	SetConsoleCursorPosition(handle, position);
	std::cout << "  " << under->description;

	// set the view port
	SDL_RenderSetViewport(renderer_g, &statsView_g);

	// todo: move to draw_stats
	std::string infoStr;
	infoStr.append(under->description);
	Texture text;
	text.loadFromRenderedText(infoStr, textColor_g);
	text.render(0, INSPECTINFO * TEXTSPACE, NULL);
}
