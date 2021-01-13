#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Global_Map.h"

Weapon::Weapon(){
	defense = 0;
	magic = 0;
	type = 0;
	setBlocking(0);
}

Weapon::Weapon(int defense, int magic, int type) {
	this->defense = defense;
	this->magic = magic;
	this->type = type;
	setBlocking(0);
}

Weapon::~Weapon(){
}

int Weapon::playerInteract() {
	if (global_map->player->getWeapon() != NULL) {
		global_map->player->setUnder(global_map->player->getWeapon());
		global_map->player->getUnder()->setUnder(getUnder());
	}
	else {
		global_map->player->setUnder(getUnder());
		setUnder(NULL);
	}
	global_map->player->setWeapon(this);
	global_map->player->drawStats(WEAPON);

	return 0;
}
