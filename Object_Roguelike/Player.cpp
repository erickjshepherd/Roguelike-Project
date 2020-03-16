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
	total_rooms = 20;
	max_room_size = 15;
	min_room_size = 5;
	room_overlap = 0;

	// set attack character
	attack_char = 'x';

	// set map and location variables
	view_distance = 10;
	consoleX = view_distance * 2;
	consoleY = view_distance;
	view_start = location - view_distance;
	view_start = view_start - (size * view_distance);
}

// player turn function
// reads key input and selects a direction to move
void Player::turn() {

	int direction = 0;
	int nextLocation = 0;
	int move_success;

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
	move_success = Move(direction);

	// Interact if can't move
	if (move_success == -1 && direction != 0) {

		int target = -1;

		if (direction == 1) {
			target = location - size;
		}
		else if (direction == 2) {
			target = location + size;
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

	int total_size = size*size;
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
			xy += size;
			xy++;

			xy += y * size;
			xy += x;

			// don't read out of range
			if (xy >= 0 && xy < (size * size)) {
				
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
			xy += y * size;
			xy += x;

			if (xy >= 0 && xy < (size * size)) {
				
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
			xy += y * size;
			xy += x;

			if (xy >= 0 && xy < (size * size)) {
				
				if (Acceptable(global_map->map[xy]->icon)) {

					std::cout << global_map->map[xy]->icon;
					std::cout << ' ';
				}
			}
		}

		if (y == 0) {
			std::cout << "  Health: " << health << "   ";
		}
		else if (y == 1) {
			std::cout << "  Strength: " << strength << "  ";
		}
		else if (y == 2) {
			std::cout << "  Floor: " << global_map->level << "   ";
		}
		else if (y == 3) {
			std::cout << "  Rooms: " << global_map->actual_total_rooms << "   ";
		}
		else if (y == 5) {
			if (weapon != NULL) {
				std::cout << "  Weapon: " << weapon->name << "   ";
			}
			else {
				std::cout << "  Weapon: none   ";
			}
		}
		std::cout << std::endl;
	}

	global_map->Draw_Events();
}

// create a new level
// input: 0 if random, 1 if town
void Player::Get_New_Level(int level) {

	Map* next = new Map(size, total_rooms, max_room_size, min_room_size, room_overlap, 0, level);
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
	view_start = view_start - (size * view_distance);

	// set the player in the map
	under = global_map->map[location];
	global_map->map[location] = this;

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

		nextLocation = location - size;

		if (global_map->map[location - size]->blocking == 0) {

			// get the new under tile
			Tile* new_under = global_map->map[location - size];

			// update map
			global_map->map[location - size] = this;
			global_map->map[location] = under;
			location -= size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			// set the new under tile
			under = new_under;

			// update camera position and redraw screen
			if (inView() == false) {

				view_start -= size * view_distance;
				while (view_start < 0) {
					view_start += size;
				}

				ClearScreen();
				Draw_Player_View();
				setCoordinates();
			}

			success = true;

		}
	}
	else if (direction == 2) {

		nextLocation = location + size;

		if (global_map->map[location + size]->blocking == 0) {

			Tile* new_under = global_map->map[location + size];

			global_map->map[location + size] = this;
			global_map->map[location] = under;
			location += size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, under->icon);
			setCoordinates();
			updateScreen(consoleX, consoleY, icon);

			// set the under tile
			under = new_under;

			if (inView() == false) {

				view_start += size * view_distance;
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

int Player::getDamage(){
	int damage = this->strength;
	if (this->weapon != NULL) {
		damage += this->weapon->damage;
	}
	return damage;
}

void Player::takeDamage(int amount) {
	if (health < amount) {
		health = 0;
		// player is dead
	}
	else {
		health = health - amount;
	}

	updateHealth();
}

void Player::updateHealth() {
	
	short view_size = (short)(view_distance * 2) + 1;
	short x = 2 * view_size;
	short y = 0;

	// set console cursor
	COORD position = { x, y };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	// print the character
	std::cout << "  Health: " << health << "   ";

	// set console cursor
	position = { view_size, view_size };
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
}

void Player::updateWeapon() {

	short view_size = (short)(view_distance * 2) + 1;
	short x = 2 * view_size;
	short y = 5;

	// set console cursor
	COORD position = { x, y };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	// print the character
	std::cout << "  Weapon: " << weapon->name << "   ";

	// set console cursor
	position = { view_size, view_size };
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
}

// destructor
Player::~Player(){


}
