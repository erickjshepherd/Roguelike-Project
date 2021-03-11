#include "stdafx.h"
#include "Wall.h"

Wall::Wall() {
	setBlocking(1);
	setIcon('#');
	setName("wall");
	setDescription("It's a wall");

	spriteType = BRICK2_W;
}

Wall::~Wall() {

}

void Wall::render(int x, int y, int colorIn) {
	SDL_RenderSetViewport(renderer_g, &mapView_g);

	if (getSpriteVersion() != -1) {
		
		if (colorIn == -1) {
			colorIn = getColor();
		}

		// get the sprite sheet
		Texture* spriteSheet = tileSets_g[WALLPATH * NUMCOLORS + colorIn];

		// get the location on the sheet
		int clipX = 0;
		int clipY = 48;
		// wall sprites are in rows of 6
		int xOffset = getSpriteVersion() % 6;
		int yOffset = getSpriteVersion() / 6;
		// wall sets are in columns of 16
		int typeXOffset = spriteType / TILE_SOURCE_SIZE;
		int typeYOffset = spriteType % TILE_SOURCE_SIZE;
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

int Wall::playerInteract() {
	return -1;
}
