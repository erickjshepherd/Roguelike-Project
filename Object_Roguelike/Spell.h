#pragma once
#include "Tile.h"
#include <string>

class Spell : public Tile {
public:
	Spell();
	~Spell();

	int damage;
	int location;
	int cd; // todo: make cd matter
	int cdCount;
	int effect;
	int intensity;
	int range;

	int currentDirection;
	int selecting;

	void Player_Interact();
	virtual int Cast();
	virtual void Flash();
	void dmgLine(int direction, int range, int damage, int effect, int intensity);
	void flashLine(int direction, int range, char flashChar);
};
