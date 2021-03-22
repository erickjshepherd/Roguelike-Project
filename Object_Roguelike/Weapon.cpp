#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Global_Map.h"

Weapon::Weapon(){
	defense = 0;
	magic = 0;
	setBlocking(0);
}

Weapon::Weapon(int defense, int magic, int type, int location) {

	// get the weapon template from the type
	weaponTemplate wTemplate = getWeaponTemplate(type);

	this->defense = defense;
	this->magic = magic;
	this->type = wTemplate.type;
	this->location = location;
	setBlocking(0);
	setSpritePath(wTemplate.spritePath);
	setSpriteSheetW(wTemplate.spriteSheetW);
	setSprite(wTemplate.sprite);

	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			this->hit[x][y] = wTemplate.hit[x][y];
			this->damage[x][y] = wTemplate.damage[x][y];
		}
	}

	// set up the spells name and description
	generateInfo();
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

void Weapon::generateInfo() {
	std::string weaponDescription;
	if (type == DAGGER) {
		setName("Dagger");
	}
	else if (type == LONGSWORD) {
		setName("Longsword");
	}

	weaponDescription = "Attack Grid: \n";
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			weaponDescription += std::to_string(damage[x][y]);
		}
		weaponDescription += "\n";
	}
	weaponDescription += " \n";
	weaponDescription += "Defense: " + std::to_string(defense) + "\n";
	weaponDescription += "Magic: " + std::to_string(magic) + "\n";
	setDescription(weaponDescription);
}

weaponTemplate Weapon::getWeaponTemplate(int type) {
	weaponTemplate temp;
	temp.type = type;
	if (type == DAGGER) {
		int hit_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
		int damage_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
		int x, y;
		for (x = 0; x < 3; x++) {
			for (y = 0; y < 3; y++) {
				temp.hit[x][y] = hit_init[x][y];
				temp.damage[x][y] = damage_init[x][y];
			}
		}
		temp.spritePath = SHORTWEPPATH;
		temp.spriteSheetW = 8;
		temp.sprite = 0;
	}
	if (type == LONGSWORD) {
		int hit_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
		int damage_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
		int x, y;
		for (x = 0; x < 3; x++) {
			for (y = 0; y < 3; y++) {
				temp.hit[x][y] = hit_init[x][y];
				temp.damage[x][y] = damage_init[x][y];
			}
		}
		temp.spritePath = MEDWEPPATH;
		temp.spriteSheetW = 6;
		temp.sprite = 0;
	}
	return temp;
}
