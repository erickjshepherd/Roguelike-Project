#include "stdafx.h"
#include "Monsters.h"

// slime constructor
Slime::Slime() {
	name = std::string("slime");
	health = 20;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	icon = 'S';
	location = 0;
	loot[0] = 0; // 1/10 to drop dagger
	loot[1] = 1; // 1/10 to drop freeze
}

Slime::Slime(int location) {
	name = std::string("slime");
	health = 20;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	icon = 'S';
	this->location = location;
	loot[0] = 1;
	loot[1] = 2;
}

// slime destructor
Slime::~Slime() {

}

Enemy* Get_Enemy(int location, int index) {

	if (index == 0) {
		return new Slime(location);
	}
}