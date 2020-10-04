#pragma once
#include "Spell.h"

//Tile* Get_Spell(int location, int index);

class Freeze : public Spell {
public:
	Freeze();
	Freeze(int location);
	int Cast();
};

class Burn : public Spell {
public:
	Burn();
	Burn(int location);
	int Cast();
};
