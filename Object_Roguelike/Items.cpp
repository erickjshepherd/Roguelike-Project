#include "stdafx.h"
#include "Items.h"

dagger::dagger() {
	defense = 0;
	magic = 0;
	type = 1;
	icon = 'd';
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
}

dagger::dagger(int location) {
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
	icon = 'd';
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
}

dagger::~dagger() {

}

Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new dagger(location);
	}
}
