#include "Global_Map.h"
#include "Spells.h"
#include <thread>

Freeze::Freeze() {
	setName("Freeze");
	setDescription("A close range freeze spell");
	initDamage = 5;
	effectDamage = 2;
	location = 0;
	effect = FREEZE;
	duration = 3;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(1);
}

Freeze::Freeze(int location) {
	setName("Freeze");
	setDescription("A close range freeze spell");
	setIcon('$');
	initDamage = 5;
	effectDamage = 2;
	this->location = location;
	effect = FREEZE;
	duration = 3;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(1);
}

int Freeze::Cast() {
	return castLine();
}

Burn::Burn() {
	setName("Burn");
	setDescription("A close range burn spell");
	setIcon('$');
	initDamage = 5;
	effectDamage = 2;
	location = 0;
	effect = BURN;
	duration = 6;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(11);
}

Burn::Burn(int location) {
	setName("Burn");
	setDescription("A close range burn spell");
	setIcon('$');
	initDamage = 5;
	effectDamage = 2;
	this->location = location;
	effect = BURN;
	duration = 6;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(11);
}

int Burn::Cast() {
	return castLine();
}
