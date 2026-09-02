#include "stdafx.h"
#include "Abilities.h"

void abilityFreeze(Tile* source, int length, int damage, Tile* target) {
	target->frozenDamage = damage;
	target->frozenLength = length;
	target->spellSource = source;
}
