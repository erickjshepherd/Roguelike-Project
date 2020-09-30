#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Global_Map.h"

Weapon::Weapon(){
	defense = 0;
	magic = 0;
	type = 0;
	blocking = 0;
}

Weapon::Weapon(int defense, int magic, int type) {
	this->defense = defense;
	this->magic = magic;
	this->type = type;
	blocking = 0;
}

Weapon::~Weapon(){
}

void Weapon::Player_Interact() {
	if (global_map->player->getWeapon() != NULL) {
		global_map->player->under = global_map->player->getWeapon();
		global_map->player->under->under = this->under;
	}
	else {
		global_map->player->under = this->under;
		this->under = NULL;
	}
	global_map->player->setWeapon(this);
	global_map->player->drawStats(WEAPON);
}
