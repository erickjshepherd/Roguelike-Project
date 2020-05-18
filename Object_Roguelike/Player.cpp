#include "stdafx.h"
#include "Player.h"
#include "Global_Map.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Console.h"
#include <list>
#include "Items.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13

// player class
Player::Player(){
	
	// set player state
	blocking = 1;
	icon = '@';
	health = 100;
	strength = 10;
	
	// set initial map generation variables
	size = 100;
	totalRooms = 20;
	maxRoomSize = 15;
	minRoomSize = 5;
	maxTunnelSize = 10;
	minTunnelSize = 4;
	roomOverlap = 0;

	// set attack character
	attack_char = 'x';

	// set map and location variables
	view_distance = 10;
	consoleX = view_distance * 2;
	consoleY = view_distance;
	view_start = location - view_distance;
	view_start = view_start - (size * view_distance);

	weapon = new dagger();
}

// player turn function
// reads key input and selects a direction to move
void Player::turn() {

	int direction = 0;
	int nextLocation = 0;
	int move_success, attack_success;

	// get the key press
	direction = _getch();
	if (direction == 224) {
		direction = _getch();
	}
	
	// get the direction and attempt to move
	if (direction == KEY_UP) {
		direction = 1;
	}
	else if (direction == KEY_DOWN) {
		direction = 2;
	}
	else if (direction == KEY_LEFT) {
		direction = 3;
	}
	else if (direction == KEY_RIGHT) {
		direction = 4;
	}
	else if (direction == ENTER) {
		direction = 0;
		under->Player_Interact();
	}
	else {
		direction = 0;
	}

	// try to attack and then try to move
	attack_success = attack(direction);
	if (attack_success == 0) {
		move_success = Move(direction);
	}
	
	// Interact if can't attack or move
	if (attack_success != 1 && move_success == -1 && direction != 0) {

		int target = -1;

		if (direction == 1) {
			target = location - global_map->size;
		}
		else if (direction == 2) {
			target = location + global_map->size;
		}
		else if (direction == 3) {
			target = location - 1;
		}
		else if (direction == 4) {
			target = location + 1;
		}
		global_map->map[target]->Player_Interact();

		// free memory of dead enemies
		//while (global_map->Dead_Enemies.size() > 0) {
			//Enemy* dead = global_map->Dead_Enemies.back();
			//global_map->Dead_Enemies.pop_back();
		//}
	}
	// Do post move actions
	else {
		
		// Generate a new level when moving onto E
		if (under->icon == 'E') {

			Get_New_Level(global_map->level + 1);
		}
		if (under->icon == 'd') {
			// todo: make a more generic message that pulls the name from the object
			std::string event("You see a dagger.");
			global_map->Add_Event(event);
			global_map->Draw_Events();
		}
	}
}

// get a random location in the new map to start
// input: 1 if a town map, 0 if a random map
// output: the integer location of the start point
int Player::getStart(int type) {

	int total_size = global_map->size*global_map->size;
	int location = rand() % total_size;
	bool found_start = false;

	if (type == 1) {

		location = 157;
	}

	else {

		while (found_start == false) {

			if (global_map->map[location]->icon == '.') {

				found_start = true;
			}

			else {

				location = rand() % total_size;
			}
		}
	}


	return location;
}

// is the player within the camera view?
// output: true if yes, false if no
bool Player::inView() {

	int x, y, xy;
	bool present = false;

	// get the width of the square to draw
	int view_size = (view_distance * 2) + 1;
	view_size -= 2;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			// set start to one square diagonally down/right from view_start
			xy = view_start;
			xy += global_map->size;
			xy++;

			xy += y * global_map->size;
			xy += x;

			// don't read out of range
			if (xy >= 0 && xy < (global_map->size * global_map->size)) {
				
				if (global_map->map[xy] == this) {

					present = true;
				}
			}
		}
	}

	return present;
}

// update the screen at specific coordinates
// inputs: X and Y are console coordinates, out is printed there
// todo: does this belong in the player class?
void Player::updateScreen(int X, int Y, char out) {

	short view_size = (short)(view_distance * 2) + 1;

	short x = (short)X;
	short y = (short)Y;

	// set console cursor
	COORD position = { x, y };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	// print the character
	std::cout << out;

	// set console cursor
	position = { view_size, view_size };
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

}

// set the player screen coordinates
// output: updates consoleX and consoleY of the player class
void Player::setCoordinates() {

	int x, y, xy;

	// get the width of the square to draw
	int view_size = (view_distance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = view_start;
			xy += y * global_map->size;
			xy += x;

			if (xy >= 0 && xy < (global_map->size * global_map->size)) {
				
				if (global_map->map[xy] == this) {

					consoleX = 2 * x;
					consoleY = y;
				}
			}
		}
	}
}

// draw the players view
// output: the area around the player is drawn to the console
void Player::Draw_Player_View() {

	int x, y, xy;

	// get the width of the square to draw
	int view_size = (view_distance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = view_start;
			xy += y * global_map->size;
			xy += x;

			if (xy >= 0 && xy < (global_map->size * global_map->size)) {
				
				if (Acceptable(global_map->map[xy]->icon)) {

					std::cout << global_map->map[xy]->icon;
					std::cout << ' ';
				}
			}
		}
		std::cout << std::endl;
	}
	drawStats(-1);
	global_map->Draw_Events();
}

// create a new level
// input: 0 if random, 1 if town
void Player::Get_New_Level(int level) {

	while (1) {
		Map* next = new Map(getMapSize(), getTotalRooms(), getMaxRoomSize(), getMinRoomSize(),getRoomOverlap(), 0, level, getMaxTunnelSize(), getMinTunnelSize());
		next->player = this;

		// delete the old map
		delete(global_map);

		// set up the new map and location
		global_map = next;
		location = getStart(global_map->type);
		view_distance = 10;
		consoleX = view_distance * 2;
		consoleY = view_distance;
		view_start = location - view_distance;
		view_start = view_start - (global_map->size * view_distance);

		// set the player in the map
		under = global_map->map[location];
		global_map->map[location] = this;

		// check that the exit can be reached
		if (global_map->findExit(location)) {
			break;
		}
	}

	// update the screen
	ClearScreen();
	Draw_Player_View();
}

// move the player in one of four directions
// input: 1 is up, 2 is down, 3 is left, 4 is right
int Player::Move(int direction) {

	int nextLocation;
	bool success = false;
	
	// get the input and move if able
	if (direction == 1) {

		nextLocation = location - global_map->size;

		if (global_map->map[location - global_map->size]->blocking == 0) {

			// get the new under tile
			Tile* new_under = global_map->map[location - global_map->size];

			// update map
			global_map->map[location - global_map->size] = this;
			global_map->map[location] = under;
			location -= global_map->size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			// set the new under tile
			under = new_under;

			// update camera position and redraw screen
			if (inView() == false) {

				view_start -= global_map->size * view_distance;
				while (view_start < 0) {
					view_start += global_map->size;
				}

				ClearScreen();
				Draw_Player_View();
				setCoordinates();
			}

			success = true;

		}
	}
	else if (direction == 2) {

		nextLocation = location + global_map->size;

		if (global_map->map[location + global_map->size]->blocking == 0) {

			Tile* new_under = global_map->map[location + global_map->size];

			global_map->map[location + global_map->size] = this;
			global_map->map[location] = under;
			location += global_map->size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			// set the under tile
			under = new_under;

			if (inView() == false) {

				view_start += global_map->size * view_distance;
				ClearScreen();
				Draw_Player_View();
				setCoordinates();
			}

			success = true;
		}
	}
	else if (direction == 3) {

		nextLocation = location - 1;

		if (global_map->map[location - 1]->blocking == 0) {

			Tile* new_under = global_map->map[location - 1];

			global_map->map[location - 1] = this;
			global_map->map[location] = under;
			location--;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			// update the under tile
			under = new_under;

			if (inView() == false) {

				view_start -= view_distance;
				ClearScreen();
				Draw_Player_View();
				setCoordinates();
			}

			success = true;
		}
	}
	else if (direction == 4) {

		nextLocation = location + 1;

		if (global_map->map[location + 1]->blocking == 0) {

			Tile* new_under = global_map->map[location + 1];

			global_map->map[location + 1] = this;
			global_map->map[location] = under;
			location++;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			under = new_under;

			if (inView() == false) {

				view_start += view_distance;
				ClearScreen();
				Draw_Player_View();
				setCoordinates();
			}

			success = true;

		}

	}
	else {
		nextLocation = 0;
	}

	if (success) {
		
		return nextLocation;
	}
	else {
		return -1;
	}
}

// output: the damage the player deals with an attack
int Player::getDamage(int x, int y){
	int damage = this->strength;
	if (this->weapon != NULL) {
		damage += this->weapon->damage[x][y];
	}
	return damage;
}

// handles logic for the player taking damage
// input: the amount of damage to take
void Player::takeDamage(int amount) {
	if (health < amount) {
		health = 0;
		// player is dead
	}
	else {
		health = health - amount;
	}

	drawStats(0);
}

// input: The line number that corrosponds to the stat. -1 draws all
void Player::drawStats(int line) {
	short view_size = (short)(view_distance * 2) + 1;
	short x = 2 * view_size;
	short y;

	COORD position;
	HANDLE handle;

	// y is the line number for each stat
	for (y = 0; y < 9; y++) {
		if (y == 0 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			std::cout << "  Health: " << health << "   ";
		}
		else if (y == 1 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			std::cout << "  Strength: " << strength << "  ";
		}
		else if (y == 2 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			std::cout << "  Floor: " << global_map->level << "   ";
		}
		else if (y == 3 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			std::cout << "  Rooms: " << global_map->actual_total_rooms << "   ";
		}
		else if (y == 5 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			if (weapon != NULL) {
				std::cout << "  Weapon: " << weapon->name << "   ";
			}
			else {
				std::cout << "  Weapon: none   ";
			}
		}
		else if (y == 6 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			if (head != NULL) {
				std::cout << "  Head: " << head->name << "   ";
			}
			else {
				std::cout << "  Head: none   ";
			}
		}
		else if (y == 7 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			if (chest != NULL) {
				std::cout << "  Chest: " << chest->name << "   ";
			}
			else {
				std::cout << "  Chest: none   ";
			}
		}
		else if (y == 8 && (y == line || line == -1)) {
			position = { x, y };
			handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, position);
			if (legs != NULL) {
				std::cout << "  Legs: " << legs->name << "   ";
			}
			else {
				std::cout << "  Legs: none   ";
			}
		}
	}
}

bool Player::attack(int direction) {
	int x, y, target, priority, success;
	// up
	success = 0;
	if (direction == 1) {
		for (priority = 1; priority < 10; priority++) {
			target = location - (3 * global_map->size) - 1;
			for (x = 0; x < 3; x++) {
				target += x * global_map->size;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->Player_Attack(getDamage(x, y))) {
							success = 1;
						}
					}
					target += 1;
				}
				target = location - (3 * global_map->size) - 1;
			}
			if (success == 1) {
				break;
			}
		}
	}
	// down
	else if (direction == 2) {
		for (priority = 1; priority < 10; priority++) {
			target = location + (3 * global_map->size) + 1;
			for (x = 0; x < 3; x++) {
				target -= x * global_map->size;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->Player_Attack(getDamage(x, y))) {
							success = 1;
						}
					}
					target -= 1;
				}
				target = location + (3 * global_map->size) + 1;
			}
			if (success == 1) {
				break;
			}
		}
	}
	// left
	else if (direction == 3) {
		for (priority = 1; priority < 10; priority++) {
			target = location - 3 + global_map->size;
			for (x = 0; x < 3; x++) {
				target += x;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->Player_Attack(getDamage(x, y))) {
							success = 1;
						}
					}
					target -= global_map->size;
				}
				target = location - 3 + global_map->size;
			}
			if (success == 1) {
				break;
			}
		}
	}
	// right
	else if (direction == 4) {
		for (priority = 1; priority < 10; priority++) {
			target = location + 3 - global_map->size;
			for (x = 0; x < 3; x++) {
				target -= x;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->Player_Attack(getDamage(x, y))) {
							success = 1;
						}
					}
					target += global_map->size;
				}
				target = location + 3 - global_map->size;
			}
			if (success == 1) {
				break;
			}
		}
	}
	return success;
}

int Player::getMapSize() {
	int total = size;
	if (head != NULL) {
		total += head->totalSize;
	}
	if (chest != NULL) {
		total += chest->totalSize;
	}
	if (legs != NULL) {
		total += legs->totalSize;
	}
	return total;
}

int Player::getTotalRooms() {
	int total = totalRooms;
	if (head != NULL) {
		total += head->totalRooms;
	}
	if (chest != NULL) {
		total += chest->totalRooms;
	}
	if (legs != NULL) {
		total += legs->totalRooms;
	}
	return total;
}

int Player::getMaxRoomSize() {
	int total = maxRoomSize;
	if (head != NULL) {
		total += head->maxRoomSize;
	}
	if (chest != NULL) {
		total += chest->maxRoomSize;
	}
	if (legs != NULL) {
		total += legs->maxRoomSize;
	}
	return total;
}


int Player::getMinRoomSize() {
	int total = minRoomSize;
	if (head != NULL) {
		total += head->minRoomSize;
	}
	if (chest != NULL) {
		total += chest->minRoomSize;
	}
	if (legs != NULL) {
		total += legs->minRoomSize;
	}
	return total;
}

int Player::getMaxTunnelSize() {
	int total = maxTunnelSize;
	if (head != NULL) {
		total += head->maxTunnelSize;
	}
	if (chest != NULL) {
		total += chest->maxTunnelSize;
	}
	if (legs != NULL) {
		total += legs->maxTunnelSize;
	}
	return total;
}

int Player::getMinTunnelSize() {
	int total = minTunnelSize;
	if (head != NULL) {
		total += head->minTunnelSize;
	}
	if (chest != NULL) {
		total += chest->minTunnelSize;
	}
	if (legs != NULL) {
		total += legs->minTunnelSize;
	}
	return total;
}

int Player::getRoomOverlap() {
	return roomOverlap;
}

// destructor
Player::~Player(){


}
