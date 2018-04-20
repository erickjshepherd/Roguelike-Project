#include <time.h>
#include <cstdlib>
#include <ctime>
#include "Map.h"
#include <vector>


Map::Map(int size, int total, int max, int min, bool overlap, int mapType, int level){

	map.resize(size*size);

	this->size = size;
	this->total_rooms = total;
	this->max_room_size = max;
	this->min_room_size = min;
	this->type = mapType;
	this->level = level;

	if (overlap) {

		this->room_overlap = 1;
	}
	else {

		this->room_overlap = 0;
	}

	// get a random map
	if (mapType == 0) {

		Map_Generate();
	}

	// get preloaded map #1
	else if (mapType == 1) {


	}
}

// Generate the map
void Map::Map_Generate() {

	Fill_Map();
	Make_Rooms();
	Make_Special_Tunnel();
	Close_Special();
	Tunnel();
	Clear_Special();
	Close_Map();

	Fill_Dead_Ends();
	Fill_Dead_Ends();

	Set_Exit();
}

// Fill map with #
void Map::Fill_Map() {

	int y;
	int Total_Size = size * size;

	for (y = 0; y != Total_Size; y++) {

		map[y] = new Tile('#', 1);
	}
}

// Create randomly sized rooms
void Map::Make_Rooms() {

	int Rooms;
	int empty, outofBounds;
	int Height;
	int Width;
	int x;
	int y;
	int xy;
	actual_total_rooms = 0;

	for (Rooms = total_rooms; Rooms != 0; Rooms--) { // Run the loop for each Room

		// Get a random Location in the map
		int Total_Size = size * size;
		int Location = rand();
		Location %= Total_Size;

		// Get random room dimensions
		Height = rand();
		Height %= max_room_size;
		if (Height < min_room_size) { Height = min_room_size; }

		Width = rand();
		Width %= max_room_size;
		if (Width < min_room_size) { Width = min_room_size; }

		empty = 1; // Assume location is empty
		outofBounds = 0; // Assume location is in bounds

		// Check to see if there is already a room here
		for (y = 0; y < Height; y++) {

			for (x = 0; x < Width; x++) {

				xy = Location;
				xy += y * size;
				xy += x;

				if (xy >= (Total_Size - size)) {

					outofBounds = 1;
				}

				// only read map if in bounds
				else {

					if (map[xy]->icon == '.') {

						empty = 0;
					}
				}
			}
		}

		if (room_overlap == 1) {

			empty = 1;
		}

		// If space is empty and in bounds make the new room
		if (empty == 1 && outofBounds == 0) {

			for (y = 0; y < Height; y++) {

				for (x = 0; x < Width; x++) {

					xy = Location;
					xy += y * size;
					xy += x;

					map[xy]->icon = '.';
					map[xy]->blocking = 0;
				}
			}

			map[xy + size]->icon = 'T'; // mark tiles for future tunnels
			actual_total_rooms++;
		}
	}
}

// Create the central tunnel
void Map::Make_Special_Tunnel() {

	int Location = size + 4; // start at the top of the map

	int Bottom = size * size; // dig to lower part of the map
	Bottom -= size * 2;

	int direction;
	int amount;

	int x;

	while (Location < Bottom) {

		// choose a direction, down or right
		direction = rand();
		direction = direction % 2;

		amount = rand();
		amount = (amount % 6) + 4;

		for (x = 0; x < amount; x++) {

			if (direction == 0) { // down

				if (Location >= 0 && Location < size*size) {
					map[Location]->icon = 'o';
					Location += size;
				}
			}

			else { // right

				if (Location >= 0 && Location < size*size) {
					
					map[Location]->icon = 'o';
					Location++;
				}
			}
		}
	}
}

// close off the top and bottom with 'o'
void Map::Close_Special() {

	int x;
	int Bottom_Row;

	for (x = 0; x < size; x++) {

		map[x]->icon = 'o';

		Bottom_Row = size*size;
		Bottom_Row -= size;
		map[Bottom_Row + x]->icon = 'o';
	}


}

// Create tunnels from each room to the central tunnel
void Map::Tunnel() {

	int x = 0;
	int Total_Size = size*size;
	Total_Size -= size;

	int direction;
	int amount = 0;

	int location;
	int top_or_bot;

	while (x < Total_Size) { // scan through the map

		if (map[x]->icon == 'T') { // if the start of a tunnel is found

			location = x;
			if (location < (size*size) / 2) { // top half will dig down

				top_or_bot = 0;
			}

			else { // bottom half will dig up

				top_or_bot = 1;
			}

			while (map[location]->icon != 'o' && location < size*size && location >= 0) { // tunnel until you find 'o'

				map[location]->icon = '.'; // Place a floor tile
				map[location]->blocking = 0;

				if (amount == 0) { // choose a new direction and amount 

					direction = rand();
					direction %= 4;

					if (direction == 0 && top_or_bot == 0) { // top half can't dig up

						direction = rand();
						direction = (direction % 3) + 1;
					}

					if (direction == 3 && top_or_bot == 1) { // bottom half can't dig down

						direction = rand();
						direction %= 3;
					}

					amount = rand();
					amount %= 6;
					amount += 4;
				}

				if (direction == 0) { // up

					location -= size;
					amount--;
				}

				else if (direction == 1) { // right

					location++;
					amount--;
				}

				else if (direction == 2) { // left

					location--;
					amount--;
				}

				else { // down

					location += size;
					amount--;
				}
			}
		}

		x++;
	}
}

// Clear all 'o' characters
void Map::Clear_Special() {

	int x;

	for (x = 0; x < size*size; x++) {

		if (map[x]->icon == 'o') {

			map[x]->icon = '.';
			map[x]->blocking = 0;
		}
	}
}

// Close off the top and bottom of the map
void Map::Close_Map() {

	int x;
	int Bottom_Row;

	for (x = 0; x < size; x++) {

		if (x >= 0 && x < size*size) {
			
			map[x]->icon = '#';
			map[x]->blocking = 1;
		}

		Bottom_Row = size*size;
		Bottom_Row -= size;
		if (x >= 0 && x < size*size) {
			
			map[Bottom_Row + x]->icon = '#';
			map[Bottom_Row + x]->blocking = 1;
		}
	}


}

// Fill in dead ends
void Map::Fill_Dead_Ends() {

	int x;
	int count;

	int Total_Size = size*size;
	Total_Size -= size;
	Total_Size--;

	for (x = size; x < Total_Size; x++) { // Scan top to bottom, only gets half

		count = 0;
		if (x - size >= 0 && x + size < size * size) {
			
			if (map[x - size]->icon == '#') { // up

				count++;
			}

			if (map[x + 1]->icon == '#') { // right

				count++;
			}

			if (map[x + size]->icon == '#') { // down

				count++;
			}

			if (map[x - 1]->icon == '#') { // left

				count++;
			}

			if (count == 3 || count == 4) { // If the tile is surrounded by 3 '#' it becomes '#'

				map[x]->icon = '#';
				map[x]->blocking = 1;
			}
		}

	}

	for (x = Total_Size; x > size; x--) { // Scan bottom to top, gets the other half

		count = 0;
		
		if (x - size >= 0 && x + size < size * size) {
			
			if (map[x - size]->icon == '#') { // up

				count++;
			}

			if (map[x + 1]->icon == '#') { // right

				count++;
			}

			if (map[x + size]->icon == '#') { // down

				count++;
			}

			if (map[x - 1]->icon == '#') { // left

				count++;
			}

			if (count == 3 || count == 4) { // If the tile is surrounded by 3 '#' it bacomes '#'

				map[x]->icon = '#';
				map[x]->blocking = 1;
			}
		}

	}
}

// Place the exit
void Map::Set_Exit() {

	int total_size = size*size;
	int location = rand() % total_size;
	bool found_start = false;

	while (found_start == false) {

		if (map[location]->icon == '.') {

			found_start = true;
		}

		else {

			location = rand() % total_size;
		}
	}
	
		map[location]->icon = 'E';
		map[location]->blocking = 0;

}

// Spawn enemies on the map
void Map::Spawn_Enemies() {


}

Map::~Map(){

}
