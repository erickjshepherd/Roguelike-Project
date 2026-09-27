#include "stdafx.h"

#include <conio.h>

#include "Global_Map.h"
#include "Shared.h"
#include "Spell.h"

Spell::Spell() {
	castType = 0;
	selecting = 0;
	setBlocking(0);
	cdCount = 0;
	setIcon('$');
	initDamage = 0;
	effectDamage = 0;
	location = 0;
	cd = 0;
	effect = NOEFFECT;
	duration = 0;
	range = 0;
}

Spell::Spell(int initDamage, int effectDamage, int location, int cd, int effect, int duration, int range, int castType, int sprite) {
	selecting = 0;
	setName("Spell");
	setDescription("A magic spell");
	setBlocking(0);
	cdCount = 0;
	setIcon('$');
	this->initDamage = initDamage;
	this->effectDamage = effectDamage;
	this->location = location;
	this->cd = cd;
	this->effect = effect;
	this->duration = duration;
	this->range = range;
	this->castType = castType;
	
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(sprite);

	// set up the spells name and description
	generateInfo();
}

Spell::~Spell() {

}

// todo: create proper setters for the spells
int Spell::playerInteract() {
	int spellNum;
	int resolved = 0;
	while (resolved == 0) {
		resolved = 1;
		if (global_map->player->getSpell1() == NULL) {
			global_map->player->setUnder(getUnder());
			setUnder(NULL);
			global_map->player->setSpell1(this);
			global_map->player->drawStats(SPELL1);
		}
		else if (global_map->player->getSpell2() == NULL) {
			global_map->player->setUnder(getUnder());
			setUnder(NULL);
			global_map->player->setSpell2(this);
			global_map->player->drawStats(SPELL2);
		}
		else if (global_map->player->getSpell3() == NULL) {
			global_map->player->setUnder(getUnder());
			setUnder(NULL);
			global_map->player->setSpell3(this);
			global_map->player->drawStats(SPELL3);
		}
		else {
			resolved = 0;
			std::string event("No more spells can be learned. Select a spell to forget.");
			global_map->player->addEvent(event);
			global_map->player->drawInfoWindow();

			selecting = 1;
			while (selecting == 1) {
				spellNum = handleEvents();
				if (spellNum == EVENT_KEY_1 || spellNum == EVENT_KEY_2 || spellNum == EVENT_KEY_3 || spellNum == EVENT_KEY_ESC) {
					selecting = 0;
					resolved = 1;
				}

				// Draw the whole frame, then render it once (the backbuffer is not
				// preserved between presents). The spell lines blink on odd frames.
				updateFrameClock();
				drawFrame_g = currentFrame_g;
				global_map->player->drawPlayerView(-1);
				if (drawFrame_g != 0) {
					global_map->player->clearStats(SPELL1);
					global_map->player->clearStats(SPELL2);
					global_map->player->clearStats(SPELL3);
				}
				SDL_RenderPresent(renderer_g);
			}
			// the main loop redraws and presents after this returns

			if (spellNum == EVENT_KEY_1) {
				global_map->player->setUnder(global_map->player->getSpell1());
				global_map->player->getUnder()->setUnder(getUnder());
				global_map->player->setSpell1(this);
				global_map->player->drawStats(SPELL1);
			}
			else if (spellNum == EVENT_KEY_2) {
				global_map->player->setUnder(global_map->player->getSpell2());
				global_map->player->getUnder()->setUnder(getUnder());
				global_map->player->setSpell2(this);
				global_map->player->drawStats(SPELL2);
			}
			else if (spellNum == EVENT_KEY_3) {
				global_map->player->setUnder(global_map->player->getSpell3());
				global_map->player->getUnder()->setUnder(getUnder());
				global_map->player->setSpell3(this);
				global_map->player->drawStats(SPELL3);
			}
		}
	}

	return 0;
}

// Which map indices does this spell hit when cast from origin in direction?
// Pure geometry: the targeting overlay and Cast() both use this so they can never disagree.
std::vector<int> Spell::targetTiles(int origin, int direction) {
	std::vector<int> tiles;
	int mapSize = global_map->size;
	int mapArea = mapSize * mapSize;
	auto add = [&](int loc) {
		if (loc >= 0 && loc < mapArea) {
			tiles.push_back(loc);
		}
	};

	// step along the cast direction, and perpendicular to it
	int increment = 0;
	int sideIncrement = 0;
	if (direction == UP) {
		increment = -mapSize;
		sideIncrement = 1;
	}
	else if (direction == DOWN) {
		increment = mapSize;
		sideIncrement = 1;
	}
	else if (direction == LEFT) {
		increment = -1;
		sideIncrement = mapSize;
	}
	else if (direction == RIGHT) {
		increment = 1;
		sideIncrement = mapSize;
	}

	if (castType == LINE) {
		for (int x = 0; x < range; x++) {
			add(origin + (x + 1) * increment);
		}
	}
	else if (castType == CIRCLE) {
		int diameter = (range * 2) + 1;
		int startLocation = origin - mapSize * range - range;
		for (int y = 0; y < diameter; y++) {
			for (int x = 0; x < diameter; x++) {
				int loc = startLocation + y * mapSize + x;
				if (loc != origin) {
					add(loc);
				}
			}
		}
	}
	else if (castType == CONE) {
		for (int x = 0; x < range; x++) {
			int loc = origin + (x + 1) * increment;
			add(loc);
			for (int i = 0; i < x; i++) {
				add(loc + sideIncrement * (i + 1));
				add(loc - sideIncrement * (i + 1));
			}
		}
	}
	return tiles;
}

// Apply this spell's damage and effect to every tile in the list
void Spell::applyTo(const std::vector<int>& tiles, int direction, Tile* source) {
	for (int loc : tiles) {
		global_map->map[loc]->spellInteract(initDamage, effect, effectDamage, duration, direction, source);
	}
}

// Cast the spell. The caller has already picked the direction (see Player::targetingInput),
// so this does no input handling or drawing and returns immediately.
int Spell::Cast(int origin, int direction, Tile* source) {
	if (cdCount != 0) {
		return 0;
	}
	applyTo(targetTiles(origin, direction), direction, source);
	cdCount = cd;
	return 1;
}

// Set the name and description of a spell based on it's attributes
int Spell::generateInfo() {
	std::string spellName;
	std::string spellDescription;
	std::string spellCastType;
	// get the name/effect
	if (effect == NOEFFECT) {
		spellName = "Spell";
	}
	else if (effect == FREEZE) {
		spellName = "Freeze";
	}
	else if (effect == BURN) {
		spellName = "Burn";
	}
	else if (effect == SLOW) {
		spellName = "Slow";
	}
	else if (effect == SCARE) {
		spellName = "Scare";
	}
	else if (effect == CHARM) {
		spellName = "Charm";
	}
	else if (effect == PUSH) {
		spellName = "Push";
	}
	// get casting type
	if (castType == LINE) {
		spellCastType = "Line";
	}
	else if (castType == CIRCLE) {
		spellCastType = "Circle";
	}
	else if (castType == CONE) {
		spellCastType = "Cone";
	}
	setName(spellName);
	spellDescription = "Effect: " + spellName + "\n";
	spellDescription += "Shape: " + spellCastType + "\n";
	spellDescription += "Range: " + std::to_string(range) + "\n";
	spellDescription += "Damage: " + std::to_string(initDamage) + "\n";
	spellDescription += "Effect Damage: " + std::to_string(effectDamage) + "\n";
	spellDescription += "Effect Duration/Range: " + std::to_string(duration) + "\n";
	spellDescription += "Cooldown: " + std::to_string(cd) + "\n";

	setDescription(spellDescription);
	return 0;
}
