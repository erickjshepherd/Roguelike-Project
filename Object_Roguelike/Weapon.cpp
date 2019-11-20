#include "Weapon.h"

Weapon::Weapon(){
	damage = 0;
	defense = 0;
	magic = 0;
	type = 0;
}

Weapon::Weapon(int damage, int defense, int magic, int type) {
	this->damage = damage;
	this->defense = defense;
	this->magic = magic;
	this->type = type;
}

Weapon::~Weapon(){
}
