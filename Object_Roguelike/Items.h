#pragma once
#include "Weapon.h"

#define NUM_ITEMS 1

Tile* Get_Item(int location, int index);

class dagger : public Weapon {
	public:
		dagger();
		dagger(int location);
		~dagger();
};
