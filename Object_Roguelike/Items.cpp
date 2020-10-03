#include "stdafx.h"
#include "Items.h"
#include "Spells.h"


// Dagger functions
dagger::dagger() {
	defense = 0;
	magic = 0;
	type = 1;
	icon = 'W';
	name = std::string("dagger");
	description = std::string("A simple dagger");
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
	spritePath = SHORTWEPPATH;
	spriteSheetW = 8;
	sprite = 0;
}

dagger::dagger(int location) {
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
	icon = 'W';
	name = std::string("dagger");
	description = std::string("A simple dagger");
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
	spritePath = SHORTWEPPATH;
	spriteSheetW = 8;
	sprite = 0;
}

dagger::~dagger() {

}

// Longsword functions
longsword::longsword() {
	defense = 0;
	magic = 0;
	type = 1; // ?
	icon = 'W';
	name = std::string("longsword");
	description = std::string("A long sword");
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
	spritePath = MEDWEPPATH;
	spriteSheetW = 6;
	sprite = 0;
}

longsword::longsword(int location) {
	defense = 0;
	magic = 0;
	type = 1; // ?
	icon = 'W';
	name = std::string("longsword");
	description = std::string("A long sword");
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
	spritePath = MEDWEPPATH;
	spriteSheetW = 6;
	sprite = 0;
}

longsword::~longsword() {

}

// controls item drops
Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new dagger(location);
	}
	else if (index == 1) {
		return new Freeze(location);
	}
	else if (index == 2) {
		return new longsword(location);
	}
}
