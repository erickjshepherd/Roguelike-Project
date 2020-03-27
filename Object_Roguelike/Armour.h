#pragma once
#include "Tile.h"
#include <string>

class Armour : public Tile {
public:
	Armour();
	~Armour();

	int type;
	int defense;
	int health;
	int rooms;
	int roomSize;
	int totalSize;
	std::string name;

	void Player_Interact();
};