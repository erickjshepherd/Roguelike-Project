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

void Floor::render(int x, int y, int colorIn) {
	SDL_RenderSetViewport(renderer_g, &mapView_g);
	if (getSpriteVersion() != -1) {
		
		if (colorIn == -1) {
			colorIn = getColor();
		}

		// get the sprite sheet
		Texture* spriteSheet = tileSets_g[FLOORPATH * NUMCOLORS + colorIn];

		// get the location on the sheet
		int clipX = 0;
		int clipY = 48;
		// wall sprites are in rows of 7
		int xOffset = getSpriteVersion() % 7;
		int yOffset = getSpriteVersion() / 7;
		// wall sets are in columns of 12
		int typeXOffset = spriteType / 12;
		int typeYOffset = spriteType % 12;
		clipX += (xOffset * TILE_SOURCE_SIZE) + (typeXOffset * TILE_SOURCE_SIZE * 7);
		clipY += (yOffset * TILE_SOURCE_SIZE) + (typeYOffset * TILE_SOURCE_SIZE * 3);

		// set up the clip
		SDL_Rect* clip = new SDL_Rect();
		clip->x = clipX;
		clip->y = clipY;
		clip->w = TILE_SOURCE_SIZE;
		clip->h = TILE_SOURCE_SIZE;

		spriteSheet->render(x, y, clip);

		delete clip;
	}
}
