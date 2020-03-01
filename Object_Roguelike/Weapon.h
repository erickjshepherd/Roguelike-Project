#pragma once
#include "Tile.h"
#include <string>

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
	std::string name;

	// player interaction
	void Player_Interact();
};
