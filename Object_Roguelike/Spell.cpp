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
	cdCount = 0;
	effect = NOEFFECT;
	duration = 0;
	range = 0;
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
			global_map->Add_Event(event);
			global_map->Draw_Events();

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

int Spell::Cast() {
	return 0;
}

void Spell::dmgLine(int direction, int range, int damage, int effect, int effectDamage, int duration) {
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
			global_map->map[hitLocation]->spellInteract(damage, effect, effectDamage, duration);
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

int Spell::castLine() {
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
		dmgLine(currentDirection, range, initDamage, effect, effectDamage, duration);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
}

void Spell::dmgCircle(int range, int damage, int effect, int effectDamage, int intensity) {
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
					global_map->map[hitLocation]->spellInteract(damage, effect, effectDamage, duration);
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
int Spell::castCircle() {
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
		dmgCircle(range, initDamage, effect, effectDamage, duration);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
}
