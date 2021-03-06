#include "stdafx.h"
#include "Monsters.h"

// slime constructor
Slime::Slime() {
	setName("slime");
	maxHealth = 20;
	health = maxHealth;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	movement = 1;
	attacks = 1;
	turnPeriod = 1;
	loot[0] = 1; // 1/10 to drop freeze
	loot[1] = 2; // 1/10 to drop longsword
	loot[2] = 3; // 1/10 to drop burn
	loot[3] = 4; // 1/10 to drop slow
	loot[4] = 5; // 1/10 to drop scare
	loot[5] = 6; // 1/10 to drop charm
	loot[6] = 7; // 1/10 to drop push
	int hit_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	attackInit(hit, damage);
	setSpritePath(SLIMEPATH);
	setSprite(24);
	setSpriteSheetW(6);
}

Slime::Slime(int location) {
	setName("slime");
	maxHealth = 20;
	health = maxHealth;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	this->location = location;
	movement = 1;
	attacks = 1;
	turnPeriod = 1;
	loot[0] = 1;
	loot[1] = 2;
	loot[2] = 3;
	loot[3] = 4;
	loot[4] = 5;
	loot[5] = 6;
	loot[6] = 7;
	int hit_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	attackInit(hit_init, damage_init);
	setSpritePath(SLIMEPATH);
	setSprite(24);
	setSpriteSheetW(6);
}

// slime destructor
Slime::~Slime() {

}

Enemy* Get_Enemy(int location, int index) {

	if (index == 0) {
		return new Slime(location);
	}
}