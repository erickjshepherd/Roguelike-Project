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
