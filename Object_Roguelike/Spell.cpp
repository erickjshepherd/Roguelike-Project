#include "Global_Map.h"
#include <Windows.h>
#include <thread>
#include "Spell.h"

Spell::Spell() {
	blocking = 0;
	cdCount = 0;
	icon = '$';
}

Spell::~Spell() {

}

// todo: create proper setters for the spells
void Spell::Player_Interact() {
	int resolved = 0;
	while (resolved == 0) {
		resolved = 1;
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
			global_map->player->spell3 = this;
			global_map->player->drawStats(12);
		}
		else {
			resolved = 0;
			std::string event("No more spells can be learned. Select a spell to forget.");
			global_map->Add_Event(event);
			global_map->Draw_Events();
			selecting = 1;
			std::thread flashThread(&Spell::flashSpellsThread, this);
			int spellNum = global_map->player->selectSpell();
			selecting = 0;
			flashThread.join();
			if (spellNum == 1) {
				global_map->player->spell1 = NULL;
			}
			else if (spellNum == 2) {
				global_map->player->spell2 = NULL;
			}
			else if (spellNum == 3) {
				global_map->player->spell3 = NULL;
			}
			else {
				resolved = 1;
			}
		}
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

void Spell::flashSpellsThread() {
	while (selecting == 1) {
		global_map->player->flashSpells();
	}
}