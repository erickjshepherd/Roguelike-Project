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

	// player interaction
	int Player_Attack(int damage);

	// spell interaction
	void Spell_Interact(int damage, int effect, int intensity);

	// flash a char
	void flashChar(char flash);
};

