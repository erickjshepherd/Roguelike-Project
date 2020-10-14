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
	int maxHealth;
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
	int charmed;

	// loot table
	int loot[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	// data
	int location;
	int consoleX, consoleY;

	// functions
	void enemyTurn();
	int senseTarget();
	int senseTarget_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start, int searchBlocking, int target);
	int Move(int direction);
	void takeDamage(int amount);
	bool receiveAttack(int damage, std::string name, int faction);
	void spellInteract(int damage, int effect, int effectDamage, int intensity);
	void resetColor();
	bool attack(int direction);
	void renderHealth();
	void render(int x, int y, int colorIn);
};

