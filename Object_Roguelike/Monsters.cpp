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
}

Slime::Slime(int location) {
	name = std::string("slime");
	health = 20;
	speed = 1;
	strength = 2;
	viewDistance = 6;
	icon = 'S';
	this->location = location;
}

// slime destructor
Slime::~Slime() {

}