#include "stdafx.h"
#include <Windows.h>
#include <thread>
#include <conio.h>
#include "Global_Map.h"
#include "Spell.h"

Spell::Spell() {
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
			int prevFrame = -1;
			while (selecting == 1) {
				// Render the screen when the frame updates
				drawFrame_g = currentFrame_g;
				if (drawFrame_g != prevFrame) {
					if (drawFrame_g == 0) {
						global_map->player->drawStats(SPELL1);
						global_map->player->drawStats(SPELL2);
						global_map->player->drawStats(SPELL3);
					}
					else {
						global_map->player->clearStats(SPELL1);
						global_map->player->clearStats(SPELL2);
						global_map->player->clearStats(SPELL3);
					}
					global_map->player->drawPlayerView(0);
					SDL_RenderPresent(renderer_g);
				}
				prevFrame = drawFrame_g;

				spellNum = handleEvents();
				if (spellNum == EVENT_KEY_1 || spellNum == EVENT_KEY_2 || spellNum == EVENT_KEY_3 || spellNum == EVENT_KEY_ESC) {
					selecting = 0;
					resolved = 1;
				}
				SDL_RenderPresent(renderer_g);
			}
			global_map->player->drawStats(SPELL1);
			global_map->player->drawStats(SPELL2);
			global_map->player->drawStats(SPELL3);
			SDL_RenderPresent(renderer_g);

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

int Spell::Cast(Tile* source) {
	int result;
	if (castType == LINE) {
		result = castLine(source);
	}
	else if (castType == CIRCLE) {
		result = castCircle(source);
	}
	else if (castType == CONE) {
		result = castCone(source);
	}
	return result;
}

void Spell::dmgLine(int direction, int range, int damage, int effect, int effectDamage, int duration, Tile* source) {
	int increment = 0;
	if (direction == UP) {
		increment = -global_map->size;
	}
	else if (direction == DOWN) {
		increment = global_map->size;
	}
	else if (direction == LEFT) {
		increment = -1;
	}
	else if (direction == RIGHT) {
		increment = 1;
	}
	for (int x = 0; x < range; x++) {
		int hitLocation = global_map->player->getLocation() + (x + 1) * increment;
		if (hitLocation >= 0 && hitLocation < global_map->size * global_map->size) {
			global_map->map[hitLocation]->spellInteract(damage, effect, effectDamage, duration, direction, source);
		}
	}
}

// Updates the color of the tiles in a line coming from the player
// Inputs: direction of the line, range of the line, color of the line
void Spell::updateLineColor(int direction, int range, int color) {
	int increment = 0;
	if (direction == UP) {
		increment = -global_map->size;
	}
	else if (direction == DOWN) {
		increment = global_map->size;
	}
	else if (direction == LEFT) {
		increment = -1;
	}
	else if (direction == RIGHT) {
		increment = 1;
	}
	for (int x = 0; x < range; x++) {
		int loc = global_map->player->getLocation();
		loc += (x + 1) * increment;
		Tile* current;
		if (loc >= 0 && loc < global_map->size * global_map->size) {
			if (color == -1) {
				current = global_map->map[loc];
				while (current != NULL) {
					current->resetColor();
					current = current->getUnder();
				}
			}
			else {
				current = global_map->map[loc];
				while (current != NULL) {
					current->setColor(color);
					current = current->getUnder();
				}
			}
		}
	}
}

int Spell::getDirection() {
	int eventValue = handleEvents();
	
	if (eventValue == EVENT_QUIT) {
		selecting = 0;
		global_map->player->setQuit(1);
		return -1;
	}
	if (eventValue == EVENT_KEY_UP) {
		currentDirection = UP;
		return eventValue;
	}
	else if (eventValue == EVENT_KEY_DOWN) {
		currentDirection = DOWN;
		return eventValue;
	}
	else if (eventValue == EVENT_KEY_LEFT) {
		currentDirection = LEFT;
		return eventValue;
	}
	else if (eventValue == EVENT_KEY_RIGHT) {
		currentDirection = RIGHT;
		return eventValue;
	}
	else if (eventValue == EVENT_KEY_ESC) {
		selecting = 0;
		return eventValue;
	}
	else if (eventValue == EVENT_KEY_ENTER){
		selecting = 0;
		return eventValue;
	}
	else {
		return -1;
	}
}

int Spell::castLine(Tile* source) {
	int finalEvent = 0;
	int prevDir;
	int success = 0;
	int prevFrame = -1;
	selecting = 1;
	currentDirection = UP;

	if (cdCount != 0) {
		return 0;
	}

	while (selecting == 1) {
		prevDir = currentDirection;
		finalEvent = getDirection();

		// Render the screen when the frame updates
		drawFrame_g = currentFrame_g;
		if (drawFrame_g != prevFrame) {
			if (drawFrame_g == 0) {
				updateLineColor(currentDirection, range, -1);
			}
			else {
				updateLineColor(currentDirection, range, CAST);
			}
			global_map->player->drawPlayerView(0);
			SDL_RenderPresent(renderer_g);
		}
		prevFrame = drawFrame_g;

		// immediately flash in the new direction. Let the flash thread catch up
		if (prevDir != currentDirection) {
			updateLineColor(prevDir, range, -1);
			if (drawFrame_g == 0) {
				updateLineColor(currentDirection, range, -1);
			}
			else {
				updateLineColor(currentDirection, range, CAST);
			}
			global_map->player->drawPlayerView(0);
			SDL_RenderPresent(renderer_g);
		}
	}
	updateLineColor(currentDirection, range, -1);
	global_map->player->drawPlayerView(0);
	SDL_RenderPresent(renderer_g);

	if (finalEvent == EVENT_KEY_ENTER) {
		dmgLine(currentDirection, range, initDamage, effect, effectDamage, duration, source);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
}

void Spell::dmgCircle(int range, int damage, int effect, int effectDamage, int intensity, Tile* source) {
	int x, y, hitLocation, diameter;
	int startLocation = global_map->player->getLocation();
	startLocation -= global_map->size * range;
	startLocation -= range;

	diameter = (range * 2) + 1;
	for (y = 0; y < diameter; y++) {
		for (x = 0; x < diameter; x++) {
			hitLocation = startLocation;
			hitLocation += y * global_map->size;
			hitLocation += x;
			if (hitLocation != global_map->player->getLocation()) {
				if (hitLocation >= 0 && hitLocation < global_map->size * global_map->size) {
					global_map->map[hitLocation]->spellInteract(damage, effect, effectDamage, duration, 0, source); // todo: how to get direction for circle spells
				}
			}
		}
	}
}

void Spell::updateCircleColor(int range, int color) {
	int x, y, loc, diameter;
	int startLocation = global_map->player->getLocation();
	startLocation -= global_map->size * range;
	startLocation -= range;

	diameter = (range * 2) + 1;
	for (y = 0; y < diameter; y++) {
		for (x = 0; x < diameter; x++) {
			loc = startLocation;
			loc += y * global_map->size;
			loc += x;
			if (loc != global_map->player->getLocation()) {
				Tile* current;
				if (loc >= 0 && loc < global_map->size * global_map->size) {
					if (color == -1) {
						current = global_map->map[loc];
						while (current != NULL) {
							current->resetColor();
							current = current->getUnder();
						}
					}
					else {
						current = global_map->map[loc];
						while (current != NULL) {
							current->setColor(color);
							current = current->getUnder();
						}
					}
				}
			}
		}
	}
}

// todo: merge cast functions somehow. Seems like a lot of copied code
int Spell::castCircle(Tile* source) {
	int finalEvent = 0;
	int prevDir;
	int success = 0;
	int prevFrame = -1;
	selecting = 1;

	if (cdCount != 0) {
		return 0;
	}

	while (selecting == 1) {
		finalEvent = getDirection();

		// Render the screen when the frame updates
		drawFrame_g = currentFrame_g;
		if (drawFrame_g != prevFrame) {
			if (drawFrame_g == 0) {
				updateCircleColor(range, -1);
			}
			else {
				updateCircleColor(range, CAST);
			}
			global_map->player->drawPlayerView(0);
			SDL_RenderPresent(renderer_g);
		}
		prevFrame = drawFrame_g;
	}
	updateCircleColor(range, -1);
	global_map->player->drawPlayerView(0);
	SDL_RenderPresent(renderer_g);

	if (finalEvent == EVENT_KEY_ENTER) {
		dmgCircle(range, initDamage, effect, effectDamage, duration, source);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
}

void Spell::dmgCone(int direction, int range, int damage, int effect, int effectDamage, int intensity, Tile* source) {
	int increment = 0;
	int sideIncrement = 0;
	if (direction == UP) {
		increment = -global_map->size;
		sideIncrement = 1;
	}
	else if (direction == DOWN) {
		increment = global_map->size;
		sideIncrement = 1;
	}
	else if (direction == LEFT) {
		increment = -1;
		sideIncrement = global_map->size;
	}
	else if (direction == RIGHT) {
		increment = 1;
		sideIncrement = global_map->size;
	}
	for (int x = 0; x < range; x++) {
		// central hit
		int hitLocation = global_map->player->getLocation() + (x + 1) * increment;
		if (hitLocation >= 0 && hitLocation < global_map->size * global_map->size) {
			global_map->map[hitLocation]->spellInteract(damage, effect, effectDamage, duration, direction, source);
		}
		// side hit
		for (int i = 0; i < x; i++) {
			int sideHitLocation = hitLocation + sideIncrement * (i + 1);
			if (sideHitLocation >= 0 && sideHitLocation < global_map->size * global_map->size) {
				global_map->map[sideHitLocation]->spellInteract(damage, effect, effectDamage, duration, direction, source);
			}
			sideHitLocation = hitLocation - sideIncrement * (i + 1);
			if (sideHitLocation >= 0 && sideHitLocation < global_map->size * global_map->size) {
				global_map->map[sideHitLocation]->spellInteract(damage, effect, effectDamage, duration, direction, source);
			}
		}
	}
}

void Spell::updateConeColor(int direction, int range, int color) {
	int increment = 0;
	int sideIncrement = 0;
	if (direction == UP) {
		increment = -global_map->size;
		sideIncrement = 1;
	}
	else if (direction == DOWN) {
		increment = global_map->size;
		sideIncrement = 1;
	}
	else if (direction == LEFT) {
		increment = -1;
		sideIncrement = global_map->size;
	}
	else if (direction == RIGHT) {
		increment = 1;
		sideIncrement = global_map->size;
	}
	for (int x = 0; x < range; x++) {
		// central
		int loc = global_map->player->getLocation();
		loc += (x + 1) * increment;
		Tile* current;
		if (loc >= 0 && loc < global_map->size * global_map->size) {
			if (color == -1) {
				current = global_map->map[loc];
				while (current != NULL) {
					current->resetColor();
					current = current->getUnder();
				}
			}
			else {
				current = global_map->map[loc];
				while (current != NULL) {
					current->setColor(color);
					current = current->getUnder();
				}
			}
		}
		// side
		for (int i = 0; i < x; i++) {
			int sideLoc = loc + sideIncrement * (i + 1);
			if (sideLoc >= 0 && sideLoc < global_map->size * global_map->size) {
				if (color == -1) {
					current = global_map->map[sideLoc];
					while (current != NULL) {
						current->resetColor();
						current = current->getUnder();
					}
				}
				else {
					current = global_map->map[sideLoc];
					while (current != NULL) {
						current->setColor(color);
						current = current->getUnder();
					}
				}
			}
			sideLoc = loc - sideIncrement * (i + 1);
			if (sideLoc >= 0 && sideLoc < global_map->size * global_map->size) {
				if (color == -1) {
					current = global_map->map[sideLoc];
					while (current != NULL) {
						current->resetColor();
						current = current->getUnder();
					}
				}
				else {
					current = global_map->map[sideLoc];
					while (current != NULL) {
						current->setColor(color);
						current = current->getUnder();
					}
				}
			}
		}
	}
}

int Spell::castCone(Tile* source) {
	int finalEvent = 0;
	int prevDir;
	int success = 0;
	int prevFrame = -1;
	selecting = 1;
	currentDirection = UP;

	if (cdCount != 0) {
		return 0;
	}

	while (selecting == 1) {
		prevDir = currentDirection;
		finalEvent = getDirection();

		// Render the screen when the frame updates
		drawFrame_g = currentFrame_g;
		if (drawFrame_g != prevFrame) {
			if (drawFrame_g == 0) {
				updateConeColor(currentDirection, range, -1);
			}
			else {
				updateConeColor(currentDirection, range, CAST);
			}
			global_map->player->drawPlayerView(0);
			SDL_RenderPresent(renderer_g);
		}
		prevFrame = drawFrame_g;

		// immediately flash in the new direction. Let the flash thread catch up
		if (prevDir != currentDirection) {
			updateConeColor(prevDir, range, -1);
			if (drawFrame_g == 0) {
				updateConeColor(currentDirection, range, -1);
			}
			else {
				updateConeColor(currentDirection, range, CAST);
			}
			global_map->player->drawPlayerView(0);
			SDL_RenderPresent(renderer_g);
		}
	}
	updateConeColor(currentDirection, range, -1);
	global_map->player->drawPlayerView(0);
	SDL_RenderPresent(renderer_g);

	if (finalEvent == EVENT_KEY_ENTER) {
		dmgCone(currentDirection, range, initDamage, effect, effectDamage, duration, source);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
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
