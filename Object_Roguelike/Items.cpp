#include "stdafx.h"
#include "Items.h"
#include "Spell.h"


// Dagger functions
dagger::dagger() {
	defense = 0;
	magic = 0;
	type = 1;
	setIcon('W');
	setName("dagger");
	setDescription("A simple dagger");
	int hit_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			hit[x][y] = hit_init[x][y];
			damage[x][y] = damage_init[x][y];
		}
	}
	// sprite
	setSpritePath(SHORTWEPPATH);
	setSpriteSheetW(8);
	setSprite(0);
}

dagger::dagger(int location) {
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
	setIcon('W');
	setName("dagger");
	setDescription("A simple dagger");
	int hit_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,0,0}, {0,1,0} };
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			hit[x][y] = hit_init[x][y];
			damage[x][y] = damage_init[x][y];
		}
	}
	// sprite
	setSpritePath(SHORTWEPPATH);
	setSpriteSheetW(8);
	setSprite(0);
}

dagger::~dagger() {

}

// Longsword functions
longsword::longsword() {
	defense = 0;
	magic = 0;
	type = 1; // ?
	setIcon('W');
	setName("longsword");
	setDescription("A long sword");
	int hit_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			hit[x][y] = hit_init[x][y];
			damage[x][y] = damage_init[x][y];
		}
	}
	// sprite
	setSpritePath(MEDWEPPATH);
	setSpriteSheetW(6);
	setSprite(0);
}

longsword::longsword(int location) {
	defense = 0;
	magic = 0;
	type = 1; // ?
	setIcon('W');
	setName("longsword");
	setDescription("A long sword");
	int hit_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
	int damage_init[3][3] = { {0,0,0}, {0,1,0}, {0,1,0} };
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			hit[x][y] = hit_init[x][y];
			damage[x][y] = damage_init[x][y];
		}
	}
	// sprite
	setSpritePath(MEDWEPPATH);
	setSpriteSheetW(6);
	setSprite(0);
}

longsword::~longsword() {

}

// controls item drops
// todo: change this to use enums
Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new dagger(location);
	}
	else if (index == 1) {
		return new Spell(5, 2, location, 5, FREEZE, 3, 2, LINE, 1);
	}
	else if (index == 2) {
		return new longsword(location);
	}
	else if (index == 3) {
		return new Spell(5, 2, location, 5, BURN, 6, 2, LINE, 11);
	}
	else if (index == 4) {
		return new Spell(0, 0, location, 5, SLOW, 10, 3, CONE, 4);
	}
	else if (index == 5) {
		return new Spell(0, 0, location, 5, SCARE, 5, 1, CIRCLE, 7);
	}
	else if (index == 6) {
		return new Spell(0, 0, location, 5, CHARM, 5, 1, CIRCLE, 2);
	}
	else if (index == 7) {
		return new Spell(0, 10, location, 5, PUSH, 2, 2, LINE, 10);
	}
}
