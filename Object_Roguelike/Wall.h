#pragma once
#include "Tile.h"

class Wall : public Tile {
public:
	int spriteType;

	Wall();
	~Wall();
	void render(int x, int y, int colorIn);
	int playerInteract();
};
