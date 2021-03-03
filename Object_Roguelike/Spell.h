#pragma once
#include "Tile.h"
#include <string>

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

	int currentDirection;
	int selecting;

	int playerInteract();
	virtual int Cast();
	void dmgLine(int direction, int range, int damage, int effect, int effectDamage, int intensity);
	void updateLineColor(int direction, int range, int color);
	void dmgCircle(int range, int damage, int effect, int effectDamage, int intensity);
	void updateCircleColor(int range, int color);
	int getDirection();
	int castLine();
	int castCircle();
	void dmgCone(int direction, int range, int damage, int effect, int effectDamage, int intensity);
	void updateConeColor(int direction, int range, int color);
	int castCone();
	int generateInfo();
};
