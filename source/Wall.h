#pragma once
#include "Tile.h"

class Wall : public Tile {
public:
	int spriteType;

	Wall();
	Wall(int spriteT);
	~Wall();
	void render(int x, int y, int colorIn);
	int playerInteract();
};
