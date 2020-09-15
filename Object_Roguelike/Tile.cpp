#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"
#include "Texture.h"

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

void Tile::Spell_Interact(int damage, int effect, int intensity) {

}

int Tile::Player_Attack(int damage) {
	return 0;
}

void Tile::render(int x, int y) {

	if (under != nullptr) {
		under->render(x, y);
	}

	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[spritePath];

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
