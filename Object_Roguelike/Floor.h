#pragma once
#include "Tile.h"

class Floor : public Tile {
public:
	Floor();
	~Floor();
	void render(int x, int y, int color);
	int spriteType;
};