#include "Player.h"
#include "Global_Map.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Console.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

Player::Player(){
	
	// set player state
	blocking = 1;
	icon = '@';
	health = 100;
	
	// set initial map generation variables
	size = 100;
	total_rooms = 20;
	max_room_size = 15;
	min_room_size = 5;
	room_overlap = 0;

	// set map and location variables
	view_distance = 10;
	consoleX = view_distance * 2;
	consoleY = view_distance;
	view_start = location - view_distance;
	view_start = view_start - (size * view_distance);
}

// player turn function
void Player::turn() {

	int direction = 0;
	int nextLocation = 0;
	int move_success;

	// get the key press
	direction = _getch();
	direction = _getch();

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
	else {
		direction = 0;
	}
	move_success = Move(direction);

	// Interact if can't move
	if (move_success == -1) {
	
	}
	else {
		
		// Generate a new level when moving onto E
		if (under->icon == 'E') {

			Get_New_Level(global_map->level + 1);
		}
	}
}

// get a random location in the new map to start
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

// is the player within the camera view
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

		std::cout << std::endl;
	}

	std::cout << "Health: " << health << std::endl;
	std::cout << "Floor: " << global_map->level << std::endl;
	std::cout << "Rooms: " << global_map->actual_total_rooms << std::endl;
}

// create a new level
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

// destructor
Player::~Player(){


}
