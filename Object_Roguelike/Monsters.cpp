#include "stdafx.h"
#include "Monsters.h"

// slime constructor
Slime::Slime() {
	setName("slime");
	health = 20;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	location = 0;
	loot[0] = 0; // 1/10 to drop dagger
	loot[1] = 1; // 1/10 to drop freeze
	loot[2] = 1; // 1/10 to drop freeze
	setSpritePath(SLIMEPATH);
	setSprite(24);
	setSpriteSheetW(6);
	setColor(STANDARD);
}

Slime::Slime(int location) {
	setName("slime");
	health = 20;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	setIcon('S');
	this->location = location;
	loot[0] = 1;
	loot[1] = 2;
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