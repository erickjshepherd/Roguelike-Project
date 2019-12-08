#include "Items.h"

dagger::dagger() {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
}

dagger::dagger(int location) {
	damage = 1;
	defense = 0;
	magic = 0;
	type = 1;
	this->location = location;
}

dagger::~dagger() {

}
