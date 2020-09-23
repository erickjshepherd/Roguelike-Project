#include "Global_Map.h"
#include "Spells.h"
#include "Console.h"
#include <thread>

Freeze::Freeze() {
	name = std::string("Freeze");
	description = std::string("A close range freeze spell");
	damage = 1;
	location = 0;
	effect = 1;
	intensity = 2;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	spritePath = SCROLLPATH;
	spriteSheetW = 8;
	spriteType = 1;
}

Freeze::Freeze(int location) {
	name = std::string("Freeze");
	description = std::string("A close range freeze spell");
	damage = 1;
	this->location = location;
	effect = 1;
	intensity = 2;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	spritePath = SCROLLPATH;
	spriteSheetW = 8;
	spriteType = 1;
}

int Freeze::Cast() {
	int direction = 0;
	int finalEvent = 0;
	int success = 0;
	selecting = 1;
	currentDirection = 1;
	
	if (cdCount != 0) {
		return 0;
	}

	std::thread flashThread(&Freeze::Flash, this);
	while (selecting == 1) {
		finalEvent = getDirection();
		ClearScreen();
		global_map->player->Draw_Player_View();
		SDL_RenderPresent(renderer_g);
	}
	flashThread.join();
	ClearScreen();
	global_map->player->Draw_Player_View();
	SDL_RenderPresent(renderer_g);

	if (finalEvent == EVENT_KEY_ENTER) {
		dmgLine(currentDirection, range, damage, effect, intensity);
		success = 1;
		cdCount = cd;
	}
	else {
		success = 0;
	}
	return success;
}

void Freeze::Flash() {
	while (selecting == 1) {
		int direction = currentDirection;
		updateLineColor(currentDirection, range, CAST);
		Sleep(200);
		if (currentDirection != direction) {
			updateLineColor(direction, range, STANDARD);
		}
		updateLineColor(currentDirection, range, STANDARD);
		Sleep(200);
	}
}
