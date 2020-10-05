#pragma once
#include "Tile.h"
#include <queue>
#include <vector>
#include <string>

class Enemy : public Tile {

public:
	
	// constructor and destructor
	Enemy();
	~Enemy();

	// stats
	int health;
	int speed;
	int strength;
	int viewDistance;

	// effects
	int frozenLength;
	int frozenDamage;
	int burnedLength;
	int burnedDamage;
	int slowed;
	int scared;
	int allied;

	// loot table
	int loot[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	// data
	int location;
	int consoleX, consoleY;

	// functions
	void enemyTurn();
	int sensePlayer();
	int sensePlayer_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start, int searchBlocking);
	int Move(int direction);
	void takeDamage(int amount);
	int playerAttack(int damage);
	void spellInteract(int damage, int effect, int effectDamage, int intensity);
	void resetColor();
	bool attack(int direction);
};

