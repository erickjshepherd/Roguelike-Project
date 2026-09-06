#pragma once
#include "Tile.h"
#include <string>

enum weaponTypes {
	DAGGER,
	LONGSWORD
};

struct weaponTemplate {
	int type;
	int hit[3][3];
	int damage[3][3];
	int spritePath;
	int spriteSheetW;
	int sprite;
};

class Weapon : public Tile {
public:

	Weapon();
	Weapon(int defense, int magic, int type, int location);
	~Weapon();

	int defense;
	int magic;
	int type;
	int location;
	int hit[3][3];
	int damage[3][3];

	// player interaction
	int playerInteract();
	void generateInfo();
	weaponTemplate getWeaponTemplate(int type);
};

