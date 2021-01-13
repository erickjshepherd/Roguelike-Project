#pragma once
#include "Tile.h"
#include <string>

class Weapon : public Tile {
public:

	Weapon();
	Weapon(int defense, int magic, int type);
	~Weapon();

	int defense;
	int magic;
	int type;
	int location;
	int hit[3][3];
	int damage[3][3];

	// player interaction
	int playerInteract();
};
