#pragma once
#include <string>
#include <vector>

#include "Tile.h"

enum spellEffectEnums {
	NOEFFECT,
	FREEZE,
	BURN,
	SLOW,
	SCARE,
	CHARM,
	PUSH
};

enum spellCastTypes {
	LINE,
	CIRCLE,
	CONE
};

class Spell : public Tile {
public:
	Spell();
	Spell(int initDamage, int effectDamage, int location, int cd, int effect, int duration, int range, int castType, int sprite);
	~Spell();

	int initDamage;
	int effectDamage;
	int location;
	int cd;
	int cdCount;
	int effect;
	int duration;
	int range;
	int castType;

	int selecting;

	int playerInteract();

	// targeting //
	// map indices this spell would affect when cast from origin in direction.
	// pure: no drawing, no damage. Used by both the targeting overlay and Cast().
	std::vector<int> targetTiles(int origin, int direction);
	// apply the spell to a set of tiles
	void applyTo(const std::vector<int>& tiles, int direction, Tile* source);
	// cast from origin in direction. returns 1 on success, 0 if on cooldown
	virtual int Cast(int origin, int direction, Tile* source);

	int generateInfo();
};
