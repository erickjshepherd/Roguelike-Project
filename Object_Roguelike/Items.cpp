#include "Items.h"

dagger::dagger() {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
	icon = 'd';
}

dagger::dagger(int location) {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
	icon = 'd';
}

dagger::~dagger() {

}

Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new dagger(location);
	}
}
