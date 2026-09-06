#include "stdafx.h"
#include "Items.h"
#include "Spell.h"

// controls item drops
// todo: change this to use enums
Tile* Get_Item(int location, int index) {

	if (index == 0) {
		return new Weapon(0, 0, DAGGER, location);
	}
	else if (index == 1) {
		return new Spell(5, 2, location, 5, FREEZE, 3, 2, LINE, 1);
	}
	else if (index == 2) {
		return new Weapon(0, 0, LONGSWORD, location);
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
	else {
		return NULL;
	}
}
