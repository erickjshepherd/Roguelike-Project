#pragma once
#include "Tile.h"

class Enemy : public Tile {

public:
	
	Enemy();
	~Enemy();

	// attributes
	int health;
	int location;
	int speed;
	int consoleX, consoleY;

	// movement
	int sensePlayer();
	void Move(int direction);

	// attack


};

