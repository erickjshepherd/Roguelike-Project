#include "Wall.h"

Wall::Wall() {
	blocking = 1;
	icon = '#';
	name = "wall";
	description = "It's a wall";

	// use the default wall sprites
	spriteType = 0;
}

Wall::~Wall() {

}

void Wall::render(int x, int y) {
	if (sprite != -1) {
		// get the sprite sheet
		Texture* spriteSheet = tileSets_g[WALLPATH];

		// get the location on the sheet
		int clipX = 0;
		int clipY = 48;
		int xOffset = sprite % 6;
		int yOffset = sprite / 6;
		int typeXOffset = spriteType / 16;
		int typeYOffset = spriteType % 16;
		clipX += (xOffset * 16) + (typeXOffset * 112);
		clipY += (yOffset * 16) + (typeYOffset * 48);

		// set up the clip
		SDL_Rect* clip = new SDL_Rect();
		clip->x = clipX;
		clip->y = clipY;
		clip->w = 16;
		clip->h = 16;

		spriteSheet->render(x, y, clip);

		delete clip;
	}
}
