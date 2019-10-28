#pragma once
#include "Tile.h"
#include <queue>
#include <vector>
#include <string>

class Enemy : public Tile {

public:
	
	Enemy();
	~Enemy();

	// stats
	int health;
	int speed;
	int strength;
	int viewDistance;

	// data
	std::string name;
	int location;
	int consoleX, consoleY;

	// turn
	void enemyTurn();

	// movement
	int sensePlayer();
	int sensePlayer_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start);
	int Move(int direction);

	// attack
	void attackPlayer();

	// take damage
	void takeDamage(int amount);

	// screen
	void onScreen(int* X, int* Y);

	// player interaction
	void Player_Interact();
};

