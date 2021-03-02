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

Slow::Slow() {
	setName("Slow");
	setDescription("A close range slow spell");
	effect = SLOW;
	duration = 10;
	cd = 5;
	cdCount = 0;
	range = 3;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(4);
}

Slow::Slow(int location) {
	setName("Slow");
	setDescription("A close range slow spell");
	this->location = location;
	effect = SLOW;
	duration = 10;
	cd = 5;
	cdCount = 0;
	range = 3;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(4);
}

int Slow::Cast() {
	return castCone();
}

Scare::Scare() {
	setName("Scare");
	setDescription("A close range scare spell");
	effect = SCARE;
	duration = 5;
	cd = 5;
	cdCount = 0;
	range = 1;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(7);
}

Scare::Scare(int location) {
	setName("Scare");
	setDescription("A close range scare spell");
	this->location = location;
	effect = SCARE;
	duration = 5;
	cd = 5;
	cdCount = 0;
	range = 1;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(7);
}

int Scare::Cast() {
	return castCircle();
}

Charm::Charm() {
	setName("Charm");
	setDescription("A close range charm spell");
	effect = CHARM;
	duration = 5;
	cd = 5;
	cdCount = 0;
	range = 1;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(2);
}

Charm::Charm(int location) {
	setName("Charm");
	setDescription("A close range charm spell");
	this->location = location;
	effect = CHARM;
	duration = 5;
	cd = 5;
	cdCount = 0;
	range = 1;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(2);
}

int Charm::Cast() {
	return castLine();
}

Push::Push() {
	setName("Push");
	setDescription("A Push spell");
	effect = PUSH;
	duration = 2;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(10);
}

Push::Push(int location) {
	setName("Push");
	setDescription("A Push spell");
	this->location = location;
	effect = PUSH;
	duration = 2;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(10);
}

int Push::Cast() {
	return castLine();
}
