#include "stdafx.h"
#include "Items.h"

dagger::dagger() {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
	icon = 'd';
	name = std::string("dagger");
}

dagger::dagger(int location) {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
	icon = 'd';
	name = std::string("dagger");
}

dagger::~dagger() {

}

Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new dagger(location);
	}
}
