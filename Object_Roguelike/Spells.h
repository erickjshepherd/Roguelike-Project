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

class Slow : public Spell {
public:
	Slow();
	Slow(int location);
	int Cast();
};

class Scare : public Spell {
public:
	Scare();
	Scare(int location);
	int Cast();
};

class Charm : public Spell {
public:
	Charm();
	Charm(int location);
	int Cast();
};

class Push : public Spell {
public:
	Push();
	Push(int location);
	int Cast();
};
