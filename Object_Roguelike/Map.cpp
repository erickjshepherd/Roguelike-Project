#include "stdafx.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include "Map.h"
#include <vector>
#include <queue>
#include <string>
#include "Global_Map.h"
#include "Monsters.h"
#include "Items.h"
#include "Tile.h"
#include <iterator>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Floor.h"
#include "Wall.h"
#include "Exit.h"
#include "GUI.h"


Map::Map(int size, int total, int max, int min, bool overlap, int mapType, int level, int maxTunnel, int minTunnel){

	map.resize(size*size);

	this->size = size;
	this->total_rooms = total;
	this->max_room_size = max;
	this->min_room_size = min;
	this->type = mapType;
	this->level = level;
	this->maxTunnelSize = maxTunnel;
	this->minTunnelSize = minTunnel;

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
	Close_Map();
	Make_Rooms();
	Make_Special_Tunnel();
	Tunnel();
	Clear_Special();
	Close_Map();
	Fill_Dead_Ends();
	Fill_Dead_Ends();
	convertToClasses();
	setWallSprites();
	setFloorSprites();

	Spawn_Enemies();

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

				if (xy >= Total_Size) {
					outofBounds = 1;
				}

				// only read map if in bounds
				else {

					if (map[xy]->getIcon() == '.') {
						empty = 0;
					}

					// do not generate rooms on map borders
					if (map[xy]->getBorder()) {
						outofBounds = 1;
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

					map[xy]->setIcon('.');
					map[xy]->setBlocking(0);
				}
			}

			map[xy]->setIcon('T'); // mark tiles for future tunnels
			actual_total_rooms++;
		}
	}
}

// Create the central tunnel
void Map::Make_Special_Tunnel() {

	int Location = size + 1; // start at the top of the map

	int Bottom = size * size; // dig to lower part of the map
	Bottom -= 2;
	Bottom -= size;

	int direction;
	int amount;
	int x;

	specialTunnelY.push_back(0);
	specialTunnelY.push_back(Location / size);
	while (Location < Bottom) {

		// choose a direction: down, right, left
		direction = rand();
		direction = direction % 2;

		amount = rand();
		amount = (amount % 6) + 4;

		for (x = 0; x < amount; x++) {

			// down
			if (direction == 0) {

				if (Location >= 0 && Location < size*size) {
					Location += size;
					if (map[Location]->getBorder()) {
						Location -= size;
						break;
					}
					map[Location]->setIcon('o');
				}
			}

			// right
			else {

				if (Location >= 0 && Location < size*size) {
					Location++;
					if (map[Location]->getBorder()) {
						Location--;
						break;
					}
					specialTunnelY.push_back(Location / size);
					map[Location]->setIcon('o');
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

		map[x]->setIcon('o');

		Bottom_Row = size*size;
		Bottom_Row -= size;
		map[Bottom_Row + x]->setIcon('o');
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

	// scan through the map
	while (x < Total_Size) {

		// start digging at marked tiles
		if (map[x]->getIcon() == 'T') {

			location = x;

			// place a floor tile
			map[location]->setIcon('.');
			map[location]->setBlocking(0);

			// top half will dig down
			if ((location / size) < specialTunnelY[location % size]) {

				top_or_bot = 0;
			}

			// bottom half will dig up
			else {

				top_or_bot = 1;
			}

			// tunnel until you find 'o'
			while (map[location]->getIcon() != 'o' && location < size*size && location >= 0) {

				// choose a new direction and amount 
				direction = rand();
				direction %= 4;

				// top half can't dig up
				if (direction == 0 && top_or_bot == 0) {
					direction = rand();
					direction = (direction % 3) + 1;
				}

				// bottom half can't dig down
				if (direction == 3 && top_or_bot == 1) {
					direction = rand();
					direction %= 3;
				}

				amount = rand();
				amount %= maxTunnelSize;
				if (amount < minTunnelSize) {
					amount = minTunnelSize;
				}

				int digCount;
				for (digCount = 0; digCount < amount; digCount++) {
					
					// up
					if (direction == 0) {
						location -= size;
						if (map[location]->getBorder()) {
							location += size;
							break;
						}
						if (map[location]->getIcon() == 'o') {
							break;
						}
						map[location]->setIcon('.');
						map[location]->setBlocking(0);
					}

					// right
					else if (direction == 1) {
						location++;
						if (map[location]->getBorder()) {
							location--;
							break;
						}
						if (map[location]->getIcon() == 'o') {
							break;
						}
						map[location]->setIcon('.');
						map[location]->setBlocking(0);
					}

					// left
					else if (direction == 2) {
						location--;
						if (map[location]->getBorder()) {
							location++;
							break;
						}
						if (map[location]->getIcon() == 'o') {
							break;
						}
						map[location]->setIcon('.');
						map[location]->setBlocking(0);
					}

					// down
					else {
						location += size;
						if (map[location]->getBorder()) {
							location -= size;
							break;
						}
						if (map[location]->getIcon() == 'o') {
							break;
						}
						map[location]->setIcon('.');
						map[location]->setBlocking(0);
					}
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

		if (map[x]->getIcon() == 'o') {

			map[x]->setIcon('.');
			map[x]->setBlocking(0);
		}
	}
}

// Close off all sides ofthe map
void Map::Close_Map() {

	int x;
	int Bottom_Row;
	int current_b;
	int current_l;
	int current_r;

	for (x = 0; x < size; x++) {

		// top	
		map[x]->setIcon('#');
		map[x]->setBlocking(1);
		map[x]->setBorder(1);

		// bottom
		Bottom_Row = size*size;
		Bottom_Row -= size;
		current_b = Bottom_Row + x;
		if (current_b >= 0 && current_b < size*size) {
			
			map[current_b]->setIcon('#');
			map[current_b]->setBlocking(1);
			map[current_b]->setBorder(1);
		}

		// left
		current_l = x * size;
		if (current_l >= 0 && current_l < size*size) {
			map[current_l]->setIcon('#');
			map[current_l]->setBlocking(1);
			map[current_l]->setBorder(1);
		}

		// right
		current_r = (x + 1) * size - 1;
		if (current_r >= 0 && current_r < size*size) {
			map[current_r]->setIcon('#');
			map[current_r]->setBlocking(1);
			map[current_r]->setBorder(1);
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
			
			if (map[x - size]->getIcon() == '#') { // up

				count++;
			}

			if (map[x + 1]->getIcon() == '#') { // right

				count++;
			}

			if (map[x + size]->getIcon() == '#') { // down

				count++;
			}

			if (map[x - 1]->getIcon() == '#') { // left

				count++;
			}

			if (count == 3 || count == 4) { // If the tile is surrounded by 3 '#' it becomes '#'

				map[x]->setIcon('#');
				map[x]->setBlocking(1);
			}
		}

	}

	for (x = Total_Size; x > size; x--) { // Scan bottom to top, gets the other half

		count = 0;
		
		if (x - size >= 0 && x + size < size * size) {
			
			if (map[x - size]->getIcon() == '#') { // up

				count++;
			}

			if (map[x + 1]->getIcon() == '#') { // right

				count++;
			}

			if (map[x + size]->getIcon() == '#') { // down

				count++;
			}

			if (map[x - 1]->getIcon() == '#') { // left

				count++;
			}

			if (count == 3 || count == 4) { // If the tile is surrounded by 3 '#' it bacomes '#'

				map[x]->setIcon('#');
				map[x]->setBlocking(1);
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
		if (map[location]->getIcon() == '.') {

			found_start = true;
		}
		else {

			location = rand() % total_size;
		}
	}

	Tile* under = map[location];
	map[location] = new Exit();
	map[location]->setUnder(under);
}

// Spawn enemies on the map
void Map::Spawn_Enemies() {

	int x, location, index;
	int Total_Size = size * size;
	Tile* under;

	for (x = 0; x < size; x++) {
		location = rand();
		location %= Total_Size;
		while (this->map[location]->getIcon() != '.') {
			location = rand();
			location %= Total_Size;
		}
		under = this->map[location];
		index = rand() % NUM_MONSTERS;
		Enemy* enemy = Get_Enemy(location, index);
		this->map[location] = enemy;
		this->map[location]->setUnder(under);
		this->Enemy_List.push_back(enemy);
	}
}

void Map::Spawn_Items() {
	int x, location;
	int Total_Size = size * size;
	Tile* under;

	for (x = 0; x < 20; x++) {
		
	}
}

// iterate through each enemy on the map and move them
void Map::Enemy_Turn() {
	int x, enemies;
	enemies = this->Enemy_List.size();
	for (x = 0; x < enemies; x++) {
		if (Enemy_List[x] != NULL) {
			Enemy_List[x]->enemyTurn();
		}
	}
}

void Map::Add_Event(std::string event) {
	this->events.push_back(event);
	if (this->events.size() > NUM_EVENTS) {
		this->events.pop_front();
	}
}

void Map::Draw_Events() {
	std::list<std::string>::iterator current;
	int textSpace = (getTextSpace() * 3) / 2;

	clearEvents();
	drawBackground(2);
	int eventY = 0;
	SDL_RenderSetViewport(renderer_g, &eventsView_g);
	for (current = this->events.begin(); current != this->events.end(); current++) {
		Texture text;
		text.loadFromRenderedText(*current, textColor_g, -1);
		text.render(0, eventY, NULL);
		eventY += textSpace;
	}
}

void Map::clearEvents() {
	SDL_RenderSetViewport(renderer_g, &eventsView_g);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = eventsView_g.h;
	rect.w = eventsView_g.w;
	clearRect(rect);
}

int Map::findExit_BFS(std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start) {
	if (nodes.size() == 0) {
		return -1;
	}

	// get the current node
	int current_node = nodes.front();
	nodes.pop();
	int previous_node = parent_nodes.front();
	parent_nodes.pop();

	if (map[current_node]->getIcon() == 'E') {
		return previous_node;
	}

	// add the surrounding nodes
	int x, next_node;
	for (x = 0; x < 4; x++) {
		if (x == 0) {
			next_node = current_node - size;
		}
		else if (x == 1) {
			next_node = current_node + size;
		}
		else if (x == 2) {
			next_node = current_node - 1;
		}
		else {
			next_node = current_node + 1;
		}

		// add unvisited, non-blocking nodes to the search list
		// throws error
		if (map[next_node]->getBlocking() == 0 || (map[next_node]->getIcon() == 'E')) {
			int isVisited, y;
			isVisited = 0;
			for (y = 0; y < visited.size(); y++) {
				if (visited[y] == next_node) {
					isVisited = 1;
				}
			}

			if (isVisited == 0) {
				nodes.push(next_node);
				parent_nodes.push(current_node);
				visited.push_back(next_node);
			}
		}
	}

	// recursively call function, return the next parent in the chain
	int result = findExit_BFS(nodes, parent_nodes, visited, start);
	if (result == current_node) {
		if (previous_node == start) {
			return result;
		}
		else {
			return previous_node;
		}
	}
	else {
		return result;
	}
}

int Map::findExit(int start) {
	std::queue<int> nodes;
	std::queue<int> parent_nodes;
	std::vector<int> visited;

	nodes.push(start);
	parent_nodes.push(-1);
	visited.push_back(start);

	return findExit_BFS(nodes, parent_nodes, visited, start);
}

void Map::convertToClasses() {
	int y, border;
	int Total_Size = size * size;

	for (y = 0; y != Total_Size; y++) {
		border = map[y]->getBorder();
		if (map[y]->getIcon() == '#') {
			delete map[y];
			map[y] = new Wall();
			map[y]->setBorder(border);
		}
		else if (map[y]->getIcon() == '.') {
			delete map[y];
			map[y] = new Floor();
			map[y]->setBorder(border);
		}
	}
}

void Map::setWallSprites() {
	int t, x, y;
	int Total_Size = size * size;

	// set surrounded walls to have no sprite (-1)
	for (t = 0; t < Total_Size; t++) {
		if (map[t]->getIcon() == '#') {
			// set border tile sprites
			if (map[t]->getBorder() == 1) {
				map[t]->setSpriteVersion(-1);
				// top left
				if (t == 0) {
					if (map[size + 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(0);
					}
				}
				// top right
				else if (t == size - 1) {
					if (map[(t + size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(2);
					}
				}
				// bottom left
				else if (t == size * (size - 1)) {
					if (map[(t - size) + 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(12);
					}
				}
				// bottom right
				else if (t == (size * size) - 1) {
					if (map[(t - size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(14);
					}
				}
				// top row
				else if (t < size && t % size != 0 && t % size != (size - 1)) {
					// horizontal line
					if (map[t + size]->getIcon() != '#') {
						map[t]->setSpriteVersion(1);
					}
					// top T-section
					else if (map[t + size + 1]->getIcon() != '#' && map[(t + size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(4);
					}
					// top left corner
					else if (map[t + size + 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(0);
					}
					// top right corner
					else if (map[(t + size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(2);
					}
				}
				// bottom row
				else if (t >= size * (size - 1) && t % size != 0 && t % size != (size - 1)) {
					// horizontal line
					if (map[t - size]->getIcon() != '#') {
						map[t]->setSpriteVersion(1);
					}
					// bottom T-section
					else if (map[(t - size) + 1]->getIcon() != '#' && map[(t - size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(16);
					}
					// bottom left corner
					else if (map[(t - size) + 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(12);
					}
					// bottom right corner
					else if (map[(t - size) - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(14);
					}
				}
				// left column
				else if (t >= size && t % size == 0 && t < size * (size - 1)) {
					if (map[t + 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(6);
					}
					// left T-section
					else if (map[t + 1 + size]->getIcon() != '#' && map[(t + 1) - size]->getIcon() != '#') {
						map[t]->setSpriteVersion(9);
					}
					// bottom left corner
					else if (map[(t + 1) - size]->getIcon() != '#') {
						map[t]->setSpriteVersion(12);
					}
					// top left corner
					else if (map[(t + 1) + size]->getIcon() != '#') {
						map[t]->setSpriteVersion(0);
					}
				}
				// right column
				else if (t >= size && t % size == (size - 1) && t < size * (size - 1)) {
					if (map[t - 1]->getIcon() != '#') {
						map[t]->setSpriteVersion(6);
					}
					// right T-section
					else if (map[(t - 1) + size]->getIcon() != '#' && map[(t - 1) - size]->getIcon() != '#') {
						map[t]->setSpriteVersion(11);
					}
					// bottom right corner
					else if (map[(t - 1) - size]->getIcon() != '#') {
						map[t]->setSpriteVersion(14);
					}
					// top right corner
					else if (map[(t - 1) + size]->getIcon() != '#') {
						map[t]->setSpriteVersion(2);
					}
				}
			}
			// set walls that are not surrounded to have sprite 0
			else {
				int checkPos;
				for (y = 0; y < 3; y++) {
					for (x = 0; x < 3; x++) {
						checkPos = (t - size) - 1;
						checkPos += (y * size) + x;
						if (map[checkPos]->getIcon() != '#') {
							map[t]->setSpriteVersion(0);
						}
					}
				}
			}
		}
	}
	// set tiles for remaining walls
	for (t = 0; t < Total_Size; t++) {
		if (map[t]->getIcon() == '#') {
			if (map[t]->getBorder() == 1 || map[t]->getSpriteVersion() == -1) {
				// already set
			}
			// center open
			else if (map[t - 1]->getSpriteVersion() == -1 && map[t - size]->getSpriteVersion() == -1 && map[t + 1]->getSpriteVersion() == -1 && map[t + size]->getSpriteVersion() == -1) {
				map[t]->setSpriteVersion(7);
			}
			// top left corner
			else if (map[t - 1]->getSpriteVersion() == -1 && map[t - size]->getSpriteVersion() == -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() != -1) {
				map[t]->setSpriteVersion(0);
			}
			// horizontal line
			else if (map[t - size]->getSpriteVersion() == -1 && map[t + size]->getSpriteVersion() == -1) {
				if (map[t - 1]->getSpriteVersion() != -1 || map[t + 1]->getSpriteVersion() != -1) {
					map[t]->setSpriteVersion(1);
				}
			}
			// top right corner
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() == -1 && map[t + 1]->getSpriteVersion() == -1 && map[t + size]->getSpriteVersion() != -1) {
				map[t]->setSpriteVersion(2);
			}
			// top T-section
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() == -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() != -1) {
				if (map[(t + size) - 1]->getSpriteVersion() != -1 && map[(t + size) + 1]->getSpriteVersion() != -1) {
					map[t]->setSpriteVersion(1);
				}
				else {
					map[t]->setSpriteVersion(4);
				}
			}
			// vertical line
			else if (map[t - 1]->getSpriteVersion() == -1 && map[t + 1]->getSpriteVersion() == -1) {
				if (map[t - size]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() == -1) {
					map[t]->setSpriteVersion(7);
				}
				else {
					map[t]->setSpriteVersion(6);
				}
			}
			// left T-section
			else if (map[t - 1]->getSpriteVersion() == -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() != -1) {
				if (map[(t + 1) - size]->getSpriteVersion() != -1 && map[(t + 1) + size]->getSpriteVersion() != -1) {
					map[t]->setSpriteVersion(6);
				}
				else {
					map[t]->setSpriteVersion(9);
				}
			}
			// center closed
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() != -1) {
				map[t]->setSpriteVersion(10);
			}
			// right T-section
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() == -1 && map[t + size]->getSpriteVersion() != -1) {
				if (map[(t - 1) - size]->getSpriteVersion() != -1 && map[(t - 1) + size]->getSpriteVersion() != -1) {
					map[t]->setSpriteVersion(6);
				}
				else {
					map[t]->setSpriteVersion(11);
				}
			}
			// bottom left corner
			else if (map[t - 1]->getSpriteVersion() == -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() == -1) {
				map[t]->setSpriteVersion(12);
			}
			// bottom right corner
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() == -1 && map[t + size]->getSpriteVersion() == -1) {
				map[t]->setSpriteVersion(14);
			}
			// bottom T-section
			else if (map[t - 1]->getSpriteVersion() != -1 && map[t - size]->getSpriteVersion() != -1 && map[t + 1]->getSpriteVersion() != -1 && map[t + size]->getSpriteVersion() == -1) {
				if (map[(t - size) - 1]->getSpriteVersion() != -1 && map[(t - size) + 1]->getSpriteVersion() != -1) {
					map[t]->setSpriteVersion(1);
				}
				else {
					map[t]->setSpriteVersion(16);
				}
			}
		}
	}
}

void Map::setFloorSprites() {
	int t;
	int Total_Size = size * size;

	for (t = 0; t < Total_Size; t++) {
		if (map[t]->getIcon() == '.') {
			// fully enclosed
			if (map[t - size]->getIcon() == '#' && map[t + size]->getIcon() == '#' && map[t - 1]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(5);
			}
			// top end
			else if (map[t - size]->getIcon() == '#' && map[t - 1]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(3);
			}
			// left end
			else if (map[t - size]->getIcon() == '#' && map[t + size]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(11);
			}
			// right end
			else if (map[t - size]->getIcon() == '#' && map[t + size]->getIcon() == '#' && map[t - 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(13);
			}
			// bottom end
			else if (map[t + size]->getIcon() == '#' && map[t - 1]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(17);
			}
			// top left corner
			else if (map[t - size]->getIcon() == '#' && map[t - 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(0);
			}
			// top right corner
			else if (map[t - size]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(2);
			}
			// vertical tunnel
			else if (map[t - 1]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(10);
			}
			// horizontal tunnel
			else if (map[t - size]->getIcon() == '#' && map[t + size]->getIcon() == '#') {
				map[t]->setSpriteVersion(12);
			}
			// bottom left corner
			else if (map[t + size]->getIcon() == '#' && map[t - 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(14);
			}
			// bottom right corner
			else if (map[t + size]->getIcon() == '#' && map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(16);
			}
			// top
			else if (map[t - size]->getIcon() == '#') {
				map[t]->setSpriteVersion(1);
			}
			// left
			else if (map[t - 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(7);
			}
			// right
			else if (map[t + 1]->getIcon() == '#') {
				map[t]->setSpriteVersion(9);
			}
			// bottom
			else if (map[t + size]->getIcon() == '#') {
				map[t]->setSpriteVersion(15);
			}
			// open
			else {
				map[t]->setSpriteVersion(8);
			}
		}
	}
}

Map::~Map(){
	delete player->getUnder();
	while (!map.empty()) {
		if (map.back() != player) {
			delete map.back();
		}
		map.pop_back();
	}
	while (!Enemy_List.empty()) {
		Enemy_List.pop_back();
	}
	while (!Dead_Enemies.empty()) {
		delete Dead_Enemies.back();
		Dead_Enemies.pop_back();
	}
}
