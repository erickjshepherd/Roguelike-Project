#pragma once
#include "Tile.h"

class Weapon : public Tile {
public:

	Weapon();
	Weapon(int damage, int defense, int magic, int type);
	~Weapon();

	int damage;
	int defense;
	int magic;
	int type;
	int location;

	// player interaction
	void Player_Interact();
};
