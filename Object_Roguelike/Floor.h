#pragma once
#include "Tile.h"

class Floor : public Tile {
public:
	Floor();
	Floor(int spriteT);
	Floor(int spriteT, int roomI);
	~Floor();
	void render(int x, int y, int colorIn);
	int spriteType;
};