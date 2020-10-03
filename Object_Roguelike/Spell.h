#pragma once
#include "Tile.h"
#include <string>

class Spell : public Tile {
public:
	Spell();
	~Spell();

	int damage;
	int location;
	int cd;
	int cdCount;
	int effect;
	int intensity;
	int range;

	int currentDirection;
	int selecting;

	void playerInteract();
	virtual int Cast();
	void dmgLine(int direction, int range, int damage, int effect, int intensity);
	void updateLineColor(int direction, int range, int color);
	int getDirection();
};
