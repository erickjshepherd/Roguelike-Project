#include "Spells.h"
#include <thread>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13
#define ESC 27

Freeze::Freeze() {
	name = std::string("Freeze");
	description = std::string("A close range freeze spell");
	damage = 1;
	location = 0;
	effect = 1;
	intensity = 2;
	cd = 5;
	range = 1;
}

Freeze::Freeze(int location) {
	name = std::string("Freeze");
	description = std::string("A close range freeze spell");
	damage = 1;
	this->location = location;
	effect = 1;
	intensity = 2;
	cd = 5;
	range = 1;
}

void Freeze::Cast() {
	int direction = 0;
	int cancel = 0;
	selecting = 1;
	currentDirection = 1;
	std::thread flashThread(&Freeze::Flash, this);
	while (direction != ENTER && cancel == 0) {
		direction = _getch();
		if (direction == 224) {
			direction = _getch();
		}
		if (direction == KEY_UP) {
			currentDirection = 1;
		}
		else if (direction == KEY_DOWN) {
			currentDirection = 2;
		}
		else if (direction == KEY_LEFT) {
			currentDirection = 3;
		}
		else if (direction == KEY_RIGHT) {
			currentDirection = 4;
		}
		else if (direction == ESC) {
			cancel = 1;
		}
	}
	selecting = 0;
	flashThread.join();
	if (cancel == 0) {
		dmgLine(currentDirection, range, damage, effect, intensity);
	}
}

void Freeze::Flash() {
	while (selecting == 1) {
		flashLine(currentDirection, range, 'o');
	}
}
