#pragma once
#include "Tile.h"

class Floor : public Tile {
public:
	Floor();
	Floor(int spriteT);
	~Floor();
	void render(int x, int y, int colorIn);
	int spriteType;
};