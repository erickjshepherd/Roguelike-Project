#pragma once
#include "Enemy.h"

#define NUM_MONSTERS 2

Enemy* Get_Enemy(int location, int index);

class Slime : public Enemy {
public:
	Slime();
	Slime(int location);
	~Slime();
};

class Orc : public Enemy {
public:
	Orc();
	Orc(int location);
	~Orc();
};
