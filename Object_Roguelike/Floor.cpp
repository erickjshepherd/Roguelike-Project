#include "Floor.h"

Floor::Floor() {
	setBlocking(0);
	setIcon('.');
	setName("floor");
	setDescription("");

	// sprite
	spriteType = BRICK3_F;
}

Floor::~Floor() {

}

void Floor::render(int x, int y, int color) {
	if (getSpriteVersion() != -1) {
		// get the sprite sheet
		Texture* spriteSheet = tileSets_g[FLOORPATH * NUMCOLORS + color];

		// get the location on the sheet
		int clipX = 0;
		int clipY = 48;
		// wall sprites are in rows of 7
		int xOffset = getSpriteVersion() % 7;
		int yOffset = getSpriteVersion() / 7;
		// wall sets are in columns of 12
		int typeXOffset = spriteType / 12;
		int typeYOffset = spriteType % 12;
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
