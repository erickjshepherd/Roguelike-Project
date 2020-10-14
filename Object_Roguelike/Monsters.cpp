#include "stdafx.h"
#include "Monsters.h"

// slime constructor
Slime::Slime() {
	setName("slime");
	maxHealth = 20;
	health = maxHealth;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	location = 0;
	loot[0] = 1; // 1/10 to drop freeze
	loot[1] = 2; // 1/10 to drop longsword
	loot[2] = 3; // 1/10 to drop burn
	loot[3] = 4; // 1/10 to drop slow
	loot[4] = 5; // 1/10 to drop scare
	loot[5] = 6; // 1/10 to drop charm
	setSpritePath(SLIMEPATH);
	setSprite(24);
	setSpriteSheetW(6);
	setColor(STANDARD);
}

Slime::Slime(int location) {
	setName("slime");
	maxHealth = 20;
	health = maxHealth;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	this->location = location;
	loot[0] = 1;
	loot[1] = 2;
	loot[2] = 3;
	loot[3] = 4;
	loot[4] = 5;
	loot[5] = 6;
	setSpritePath(SLIMEPATH);
	setSprite(24);
	setSpriteSheetW(6);
	setColor(STANDARD);
}

// slime destructor
Slime::~Slime() {

}

Enemy* Get_Enemy(int location, int index) {

	if (index == 0) {
		return new Slime(location);
	}
}