#include "Global_Map.h"
#include <Windows.h>
#include "Spell.h"

Spell::Spell() {
	blocking = 0;
	cdCount = 0;
	icon = '$';
}

Spell::~Spell() {

}

// todo: implement a system to choose what spell to replace when full
void Spell::Player_Interact() {
	if (global_map->player->spell1 == NULL) {
		global_map->player->under = this->under;
		this->under = NULL;
		global_map->player->spell1 = this;
		global_map->player->drawStats(10);
	}
	else if (global_map->player->spell2 == NULL) {
		global_map->player->under = this->under;
		this->under = NULL;
		global_map->player->spell2 = this;
		global_map->player->drawStats(11);
	}
	else if (global_map->player->spell3 == NULL) {
		global_map->player->under = this->under;
		this->under = NULL;
		global_map->player->spell2 = this;
		global_map->player->drawStats(12);
	}
}

int Spell::Cast() {
	return 0;
}

void Spell::Flash()
{
}

void Spell::dmgLine(int direction, int range, int damage, int effect, int intensity) {
	int increment = 0;
	if (direction == 1) {
		increment = -global_map->size;
	}
	else if (direction == 2) {
		increment = global_map->size;
	}
	else if (direction == 3) {
		increment = -1;
	}
	else if (direction == 4) {
		increment = 1;
	}
	for (int x = 0; x < range; x++) {
		int hitLocation = global_map->player->location + (x + 1) * increment;
		global_map->map[hitLocation]->Spell_Interact(damage, effect, intensity);
	}
}

// todo: don't flash over certain tile like walls
void Spell::flashLine(int direction, int range, char flashChar) {
	int increment = 0;
	int sleepTime = 200;
	int screenX, screenY;
	if (direction == 1) {
		increment = -global_map->size;
		screenX = global_map->player->consoleX;
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenY = global_map->player->consoleY - x - 1;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, flashChar);
			}
		}
		Sleep(sleepTime);
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenY = global_map->player->consoleY - x - 1;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, global_map->map[flashLocation]->icon);
			}
		}
		Sleep(sleepTime);
	}
	else if (direction == 2) {
		increment = global_map->size;
		screenX = global_map->player->consoleX;
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenY = global_map->player->consoleY + x + 1;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, flashChar);
			}
		}
		Sleep(sleepTime);
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenY = global_map->player->consoleY + x + 1;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, global_map->map[flashLocation]->icon);
			}
		}
		Sleep(sleepTime);
	}
	else if (direction == 3) {
		increment = -1;
		screenY = global_map->player->consoleY;
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenX = global_map->player->consoleX - 2 * x - 2;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, flashChar);
			}
		}
		Sleep(sleepTime);
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenX = global_map->player->consoleX - 2 * x - 2;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, global_map->map[flashLocation]->icon);
			}
		}
		Sleep(sleepTime);
	}
	else if (direction == 4) {
		increment = 1;
		screenY = global_map->player->consoleY;
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenX = global_map->player->consoleX + 2 * x + 2;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, flashChar);
			}
		}
		Sleep(sleepTime);
		for (int x = 0; x < range; x++) {
			int flashLocation = global_map->player->location + (x + 1) * increment;
			screenX = global_map->player->consoleX + 2 * x + 2;
			if (flashLocation >= 0 && flashLocation < (global_map->size * global_map->size)) {
				global_map->player->updateScreen(screenX, screenY, global_map->map[flashLocation]->icon);
			}
		}
		Sleep(sleepTime);
	}
}
