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
	if (description.length() > 0) {
		global_map->player->underDescribed = 1;
		global_map->player->drawUnderInfo(); // todo: handle more of this here instead of in player class
	}
	else {
		if (global_map->player->underDescribed == 1) {
			// todo: make member function
			global_map->player->underDescribed = 0;
			short view_size = (short)(global_map->player->view_distance * 2) + 1;
			short x = 2 * view_size;
			COORD position = { x, 14 + 1 };
			ClearLine(position, 30);
		}
	}
}

void Tile::Spell_Interact(int damage, int effect, int intensity) {

}

int Tile::Player_Attack(int damage) {
	return 0;
}

void Tile::render(int x, int y, int color) {

	if (under != nullptr) {
		under->render(x, y, color);
	}

	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[spritePath * NUMCOLORS + color];

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

	global_map->player->clearStats(14 + 1);

	COORD position = { x, 14 + 1 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);;
	SetConsoleCursorPosition(handle, position);
	std::cout << "  " << under->description;
}
