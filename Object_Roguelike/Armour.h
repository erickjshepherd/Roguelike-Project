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
	int totalRooms;
	int maxRoomSize;
	int minRoomSize;
	int maxTunnelSize;
	int minTunnelSize;
	int totalSize;

	void playerInteract();
};