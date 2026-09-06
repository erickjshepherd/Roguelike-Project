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
	int strength;
	int viewDistance;
	int idleMove;

	// speed stats
	int movement; // number of tiles that can be traversed per turn
	int attacks; // number of attacks per turn
	int turnPeriod; // how often the enemy takes turns
	int turnCount; // increases every round until equal to turnPeriod

	// attack stats
	int hit[3][3];
	int damage[3][3];

	// ability
	// parameters: intensity, damage
	void (*ability)(Tile*, int, int, Tile*);
	int abilityTrigger;
	int abilityIntensity;
	int abilityDamage;
	int abilityCooldown;
	int abilityCounter = 0;

	// loot table
	int loot[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	// data
	int location;
	int consoleX, consoleY;
	int prevDirection;

	// functions
	void enemyTurn();
	int senseTarget();
	int senseTarget_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start, int searchBlocking, int target);
	int Move(int direction);
	void takeDamage(int amount, Tile* source);
	bool receiveAttack(int damage, std::string name, int faction, Tile* source);
	void spellInteract(int damage, int effect, int effectDamage, int intensity, int direction, Tile* source);
	void resetColor();
	bool attack(int direction);
	void renderHealth();
	void render(int x, int y, int colorIn);
	int reverseDirection(int direction);
	int getIdleDirection(int direction);
	int getDamage(int x, int y);
	int inRange(int location, int target);
	void attackInit(int hit[3][3], int damage[3][3]);
	int getTarget();
	int getRelatedDirection(int direction, int relation);
	int getNewLocation(int direction);
	void forceMove(int direction, int distance, int damage, Tile* source);
	// ability functions
	void executeAbility(int trigger, Tile* target);
	bool abilityCheck();
	void executeAdjacent();
};

enum relationEnums {
	RFORWARD,
	RBACKWARD,
	RLEFT,
	RRIGHT
};

enum triggerEnums {
	TATTACK,
	TMOVE,
	TDAMAGED,
	TADJACENT,
	TDEAD
};
