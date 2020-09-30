#include <Windows.h>
#include <thread>
#include <conio.h>
#include "Global_Map.h"
#include "Spell.h"
#include <mutex>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13
#define ESC 27

Spell::Spell() {
	blocking = 0;
	cdCount = 0;
	icon = '$';
}

Spell::~Spell() {

}

// todo: create proper setters for the spells
void Spell::Player_Interact() {
	int spellNum;
	int resolved = 0;
	while (resolved == 0) {
		resolved = 1;
		if (global_map->player->getSpell1() == NULL) {
			global_map->player->under = this->under;
			this->under = NULL;
			global_map->player->setSpell1(this);
			global_map->player->drawStats(SPELL1);
		}
		else if (global_map->player->getSpell2() == NULL) {
			global_map->player->under = this->under;
			this->under = NULL;
			global_map->player->setSpell2(this);
			global_map->player->drawStats(SPELL2);
		}
		else if (global_map->player->getSpell3() == NULL) {
			global_map->player->under = this->under;
			this->under = NULL;
			global_map->player->setSpell3(this);
			global_map->player->drawStats(SPELL3);
		}
		else {
			resolved = 0;
			std::string event("No more spells can be learned. Select a spell to forget.");
			global_map->Add_Event(event);
			global_map->Draw_Events();

			selecting = 1;
			int visible = 1;
			while (selecting == 1) {
				if (visible == 0) {
					global_map->player->drawStats(SPELL1);
					global_map->player->drawStats(SPELL2);
					global_map->player->drawStats(SPELL3);
					visible = 1;
				}
				else {
					global_map->player->clearStats(SPELL1);
					global_map->player->clearStats(SPELL2);
					global_map->player->clearStats(SPELL3);
					visible = 0;
				}
				spellNum = handleEvents();
				if (spellNum == EVENT_KEY_1 || spellNum == EVENT_KEY_2 || spellNum == EVENT_KEY_3 || spellNum == EVENT_KEY_ESC) {
					selecting = 0;
					resolved = 1;
				}
				SDL_RenderPresent(renderer_g);
				Sleep(200);
			}
			global_map->player->drawStats(SPELL1);
			global_map->player->drawStats(SPELL2);
			global_map->player->drawStats(SPELL3);
			SDL_RenderPresent(renderer_g);

			if (spellNum == EVENT_KEY_1) {
				global_map->player->under = global_map->player->getSpell1();
				global_map->player->under->under = this->under;
				global_map->player->setSpell1(this);
				global_map->player->drawStats(SPELL1);
			}
			else if (spellNum == EVENT_KEY_2) {
				global_map->player->under = global_map->player->getSpell2();
				global_map->player->under->under = this->under;
				global_map->player->setSpell2(this);
				global_map->player->drawStats(SPELL2);
			}
			else if (spellNum == EVENT_KEY_3) {
				global_map->player->under = global_map->player->getSpell3();
				global_map->player->under->under = this->under;
				global_map->player->setSpell3(this);
				global_map->player->drawStats(SPELL3);
			}
		}
	}
}

int Spell::Cast() {
	return 0;
}

void Spell::dmgLine(int direction, int range, int damage, int effect, int intensity) {
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
		global_map->map[hitLocation]->Spell_Interact(damage, effect, intensity);
	}
}

// Updates the color of the tiles in a line coming from the player
// Inputs: direction of the line, range of the line, color of the line
std::mutex spellMtx;
void Spell::updateLineColor(int direction, int range, int color) {
	spellMtx.lock();
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
		global_map->map[loc]->color = color;
	}
	spellMtx.unlock();
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
