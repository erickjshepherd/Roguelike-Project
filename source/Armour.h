#pragma once
#include <string>

#include "Tile.h"

class Armour : public Tile {
public:
	Armour();
	~Armour();

	int type;
	int defense;
	int health;
	int totalRooms;
	int maxRoomSize;
	int minRoomSize;
	int maxTunnelSize;
	int minTunnelSize;
	int totalSize;

	int playerInteract();
};