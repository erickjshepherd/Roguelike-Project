#include "Global_Map.h"
#include "Spells.h"
#include <thread>

Freeze::Freeze() {
	setName("Freeze");
	setDescription("A close range freeze spell");
	damage = 1;
	location = 0;
	effect = 1;
	intensity = 3;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(1);
}

Freeze::Freeze(int location) {
	setName("Freeze");
	setDescription("A close range freeze spell");
	damage = 1;
	this->location = location;
	effect = 1;
	intensity = 3;
	cd = 5;
	cdCount = 0;
	range = 2;

	// sprite
	setSpritePath(SCROLLPATH);
	setSpriteSheetW(8);
	setSprite(1);
}

int Freeze::Cast() {
	int finalEvent = 0;
	int prevDir;
	int success = 0;
	selecting = 1;
	currentDirection = UP;
	
	if (cdCount != 0) {
		return 0;
	}

	std::thread flashThread(&Freeze::Flash, this);
	while (selecting == 1) {
		prevDir = currentDirection;
		finalEvent = getDirection();
		// immediately flash in the new direction. Let the flash thread catch up
		if (prevDir != currentDirection) {
			updateLineColor(prevDir, range, STANDARD);
			updateLineColor(currentDirection, range, CAST);
		}
		global_map->player->drawPlayerView();
		SDL_RenderPresent(renderer_g);
	}
	flashThread.join();
	global_map->player->drawPlayerView();
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
