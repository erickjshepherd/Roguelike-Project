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
#include <math.h>
#include "Shared.h"


Map::Map(int size, int total, int max, int min, int buffer, bool overlap, int mapType, int level, int maxTunnel, int minTunnel){

	map.resize(size*size);

	this->size = size;
	this->total_rooms = total;
	this->maxRoomSize = max;
	this->minRoomSize = min;
	this->type = mapType;
	this->level = level;
	this->maxTunnelSize = maxTunnel;
	this->minTunnelSize = minTunnel;
	this->roomBuffer = buffer;
	this->biome = 0;

	if (overlap) {
		this->room_overlap = 1;
	}
	else {

		this->room_overlap = 0;
	}

	// get a random map
	if (mapType == 0) {
		// try to generate the map 100 times
		int success;
		for (int x = 0; x < 100; x++) {
			success = Map_Generate();
			if (success != -1) {
				break;
			}
		}
		// todo: have a preset map for when generation fails
		if (success == -1) {
			
		}
	}

	// get preloaded map #1
	else if (mapType == 1) {


	}
}

// Generate the map
int Map::Map_Generate() {

	// create the dungeon
	Fill_Map();
	Close_Map();
	Make_Rooms();
	Make_Special_Tunnel();
	Tunnel();
	Clear_Special();
	Close_Map();
	Fill_Dead_Ends();
	Fill_Dead_Ends();

	// set the biome
	biome = determineBiome();

	// set up sprites
	convertToClasses();
	setWallSprites();
	setFloorSprites();

	// place things in the dungeon
	int playerLocation = placePlayerStart();
	if (playerLocation == -1) {
		return -1;
	}
	int exitLocation = Set_Exit();
	if (exitLocation == -1) {
		return -1;
	}
	Spawn_Enemies();
	
	// clear the icon serving as a player placeholder when everything else has been placed
	clearPlayerStart();

	return 0;
}

// Fill map with #
void Map::Fill_Map() {

	int y;
	int Total_Size = size * size;

	for (y = 0; y != Total_Size; y++) {

		map[y] = new Tile('#', 1, 0);
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

	// Run the loop for each Room
	for (Rooms = total_rooms; Rooms != 0; Rooms--) {

		// Get a random Location in the map
		int Total_Size = size * size;
		int Location = rand();
		Location %= Total_Size;

		// Get random room dimensions
		Height = rand();
		Height %= (maxRoomSize - minRoomSize) + 1;
		Height += minRoomSize;

		Width = rand();
		Width %= (maxRoomSize - minRoomSize) + 1;
		Width += minRoomSize;

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

					if (map[xy]->getIcon() == '.' || map[xy]->getIcon() == 'b') {
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
					map[xy]->setRoom(1);
				}
			}

			// keep track of room data
			int roomCenter = Location + (Width / 2) + (Height * size / 2);
			roomInfo info = { Width, Height, roomCenter, Location};
			roomData.push_back(info);

			map[xy]->setIcon('T'); // mark tiles for future tunnels
			actual_total_rooms++;

			// set up a buffer that surrounds the room
			int xLoc = Location % size;
			int yLoc = Location / size;
			int xBuff = roomBuffer;
			int yBuff = roomBuffer;
			// don't let the buffer extend outside the map
			if (xLoc < roomBuffer) {
				xBuff = xLoc;
			}
			if (yLoc < roomBuffer) {
				yBuff = yLoc;
			}
			int bufferStart = (Location - xBuff) - yBuff * size;
			// don't let the buffer start on the map borders
			if (bufferStart % size == 0) {
				bufferStart++;
				xBuff--;
			}
			if (bufferStart / size == 0) {
				bufferStart += size;
				yBuff--;
			}
			for (y = 0; y < Height + roomBuffer + yBuff; y++) {

				for (x = 0; x < Width + roomBuffer + xBuff; x++) {

					xy = bufferStart;
					xy += y * size;
					xy += x;
					
					if (xy >= Total_Size) {
						break;
					}

					if (map[xy]->getBorder()) {
						break;
					}

					if (map[xy]->getIcon() == '#') {
						map[xy]->setIcon('b');
					}
				}
			}
		}
	}
	clearRoomBuffers();
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
				amount %= (maxTunnelSize - minTunnelSize) + 1;
				amount += minTunnelSize;

				hallLengths.push_back(amount);

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
int Map::Set_Exit() {
	int location = getOpenLocation(1);
	
	if (location == -1) {
		return location;
	}

	Tile* under = map[location];
	map[location] = new Exit();
	map[location]->setUnder(under);
	return location;
}

// Spawn enemies on the map
void Map::Spawn_Enemies() {

	int x, location, enemyType;
	int Total_Size = size * size;
	Tile* under;

	for (x = 0; x < size; x++) {
		location = getOpenLocation(0);
		if (location == -1) {
			break;
		}
		under = this->map[location];
		enemyType = rand() % NUM_MONSTERS;
		Enemy* enemy = Get_Enemy(location, enemyType);
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
int Map::Enemy_Turn() {
	int x, enemies;
	enemies = this->Enemy_List.size();
	for (x = 0; x < enemies; x++) {
		if (Enemy_List[x] != NULL) {
			Enemy_List[x]->enemyTurn();
		}
	}
	return PLAYER_S;
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
	int y, border, wallSpriteT, floorSpriteT;
	int Total_Size = size * size;

	for (y = 0; y != Total_Size; y++) {
		border = map[y]->getBorder();
		wallSpriteT = getWallSet();
		floorSpriteT = getFloorSet();
		if (map[y]->getIcon() == '#') {
			delete map[y];
			map[y] = new Wall(wallSpriteT);
			map[y]->setBorder(border);
		}
		else if (map[y]->getIcon() == '.') {
			int isRoom = map[y]->getRoom();
			delete map[y];
			map[y] = new Floor(floorSpriteT, isRoom);
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

void Map::clearRoomBuffers() {
	int t;
	int Total_Size = size * size;

	for (t = 0; t < Total_Size; t++) {
		if (map[t]->getIcon() == 'b') {
			map[t]->setIcon('#');
		}
	}
}

int Map::getOpenLocation(int roomOnly) {
	int t, numLoc, locationIndex;
	int Total_Size = size * size;
	std::vector<int> openLocations;

	// get all open locations
	for (t = 0; t < Total_Size; t++) {
		if (map[t]->getIcon() == '.') {
			if ((roomOnly && map[t]->getRoom()) || (roomOnly == 0)) {
				openLocations.push_back(t);
			}
		}
	}
	numLoc = openLocations.size();

	if (numLoc == 0) {
		return -1;
	}

	locationIndex = rand() % numLoc;
	return openLocations[locationIndex];
}

int Map::placePlayerStart() {
	int location = getOpenLocation(1);
	if (location == -1) {
		return -1;
	}
	playerStart = location;
	map[location]->setIcon('@');
	return location;
}

void Map::clearPlayerStart() {
	map[playerStart]->setIcon('.');
}

// Returns a biome based on the map properties
int Map::determineBiome() {
	int numRooms = actual_total_rooms;
	int avgRoomSize = getAverageRoomSize();
	int avgHallSize = getAverageHallLength();
	int avgRoomDistance = getAverageRoomDist();

	// determine if parameters are 'big' or 'small'
	// todo: I don't love this system but it can be tweaked later
	bool largeRooms = avgRoomSize > 64;
	bool longHalls = avgHallSize > 10;
	bool sparseRooms = avgRoomDistance > 10;
	bool manyRooms = numRooms > 15;

	if (!largeRooms && !longHalls && !sparseRooms && !manyRooms) {
		biomeName = "Crystal Cave";
		return CRYSTAL_CAVE;
	}
	else if (!largeRooms && !longHalls && !sparseRooms && manyRooms) {
		biomeName = "Ice Caves";
		return ICE_CAVE;
	}
	else if (!largeRooms && !longHalls && sparseRooms && !manyRooms) {
		biomeName = "Summit";
		return SUMMIT;
	}
	else if (!largeRooms && !longHalls && sparseRooms && manyRooms) {
		biomeName = "Mountain";
		return MOUNTAIN;
	}
	else if (!largeRooms && longHalls && !sparseRooms && !manyRooms) {
		biomeName = "Basement";
		return BASEMENT;
	}
	else if (!largeRooms && longHalls && !sparseRooms && manyRooms) {
		biomeName = "Catacomb";
		return CATACOMB;
	}
	else if (!largeRooms && longHalls && sparseRooms && !manyRooms) {
		biomeName = "Sewer";
		return SEWER;
	}
	else if (!largeRooms && longHalls && sparseRooms && manyRooms) {
		biomeName = "Mine";
		return MINE;
	}
	else if (largeRooms && !longHalls && !sparseRooms && !manyRooms) {
		biomeName = "Village";
		return VILLAGE;
	}
	else if (largeRooms && !longHalls && !sparseRooms && manyRooms) {
		biomeName = "Field";
		return FIELD;
	}
	else if (largeRooms && !longHalls && sparseRooms && !manyRooms) {
		biomeName = "Cave";
		return CAVE;
	}
	else if (largeRooms && !longHalls && sparseRooms && manyRooms) {
		biomeName = "Forest";
		return FOREST;
	}
	else if (largeRooms && longHalls && !sparseRooms && !manyRooms) {
		biomeName = "House";
		return HOUSE;
	}
	else if (largeRooms && longHalls && !sparseRooms && manyRooms) {
		biomeName = "Castle";
		return CASTLE;
	}
	else if (largeRooms && longHalls && sparseRooms && !manyRooms) {
		biomeName = " Large Crystal Cave";
		return CRYSTAL_CAVE_2;
	}
	else if (largeRooms && longHalls && sparseRooms && manyRooms) {
		biomeName = "City";
		return CITY;
	}
	else {
		biomeName = "Default";
		return FIELD;
	}
}

// Returns the wall sprite based on the biome
int Map::getWallSet() {
	int wall = 0;
	if (biome == CRYSTAL_CAVE) {
		wall = GCRYSTAL2_W;
	}
	else if (biome == ICE_CAVE) {
		wall = ICE2_W;
	}
	else if (biome == SUMMIT) {
		wall = DIRT7_W;
	}
	else if (biome == MOUNTAIN) {
		wall = DIRT1_W;
	}
	else if (biome == BASEMENT) {
		wall = BRICK3_W;
	}
	else if (biome == CATACOMB) {
		wall = BRICK4_W;
	}
	else if (biome == SEWER) {
		wall = BRICK3_W;
	}
	else if (biome == MINE) {
		wall = MINE2_W;
	}
	else if (biome == CRYSTAL_CAVE_2) {
		wall = YCRYSTAL2_W;
	}
	else if (biome == FIELD) {
		wall = DIRT1_W;
	}
	else if (biome == CAVE) {
		wall = GRAVEL3_W;
	}
	else if (biome == FOREST) {
		wall = DIRT2_W;
	}
	else if (biome == HOUSE) {
		wall = WOOD1_W;
	}
	else if (biome == CASTLE) {
		wall = BRICK2_W;
	}
	else if (biome == VILLAGE) {
		wall = WOOD1_W;
	}
	else if (biome == CITY) {
		wall = BRICK1_W;
	}
	return wall;
}

// returns the floor sprite based on the biome
int Map::getFloorSet() {
	int floor = 0;
	if (biome == CRYSTAL_CAVE) {
		floor = ICE1_F;
	}
	else if (biome == ICE_CAVE) {
		floor = ICE2_F;
	}
	else if (biome == SUMMIT) {
		floor = ROCK3_F;
	}
	else if (biome == MOUNTAIN) {
		floor = ROCK2_F;
	}
	else if (biome == BASEMENT) {
		floor = BRICK3_F;
	}
	else if (biome == CATACOMB) {
		floor = BRICK4_F;
	}
	else if (biome == SEWER) {
		floor = BRICK4_F;
	}
	else if (biome == MINE) {
		floor = SPECK3_F;
	}
	else if (biome == CRYSTAL_CAVE_2) {
		floor = ICE1_F;
	}
	else if (biome == FIELD) {
		floor = GRASS2_F;
	}
	else if (biome == CAVE) {
		floor = ROCK4_F;
	}
	else if (biome == FOREST) {
		floor = GRASS3_F;
	}
	else if (biome == HOUSE) {
		floor = BRICK6_F;
	}
	else if (biome == CASTLE) {
		floor = BRICK2_F;
	}
	else if (biome == VILLAGE) {
		floor = GRASS2_F;
	}
	else if (biome == CITY) {
		floor = BRICK2_F;
	}
	return floor;
}

int Map::measureRoomDistance(roomInfo r1, roomInfo r2) {
	std::vector<int> points1;
	std::vector<int> points2;

	points1.push_back(r1.topLeft);
	points1.push_back(r1.topLeft + r1.xSize - 1);
	points1.push_back(r1.topLeft + (r1.ySize - 1) * size);
	points1.push_back(r1.topLeft + ((r1.ySize - 1) * size) + r1.xSize - 1);

	points2.push_back(r2.topLeft);
	points2.push_back(r2.topLeft + r2.xSize - 1);
	points2.push_back(r2.topLeft + (r2.ySize - 1) * size);
	points2.push_back(r2.topLeft + ((r2.ySize - 1) * size) + r2.xSize - 1);

	int dist = 0;
	int first = 1;
	for (int x = 0; x < points1.size(); x++) {
		for (int y = 0; y < points2.size(); y++) {
			int tempDist = measurePointDistance(points1[x], points2[y]);
			if (tempDist < dist || first == 1) {
				dist = tempDist;
				first = 0;
			}
		}
	}
	return dist;
}

int Map::measurePointDistance(int x, int y) {
	int row1 = x / size;
	int col1 = x % size;

	int row2 = y / size;
	int col2 = y % size;

	int colDiff;
	if (col1 > col2) {
		colDiff = col1 - col2;
	}
	else {
		colDiff = col2 - col1;
	}

	int rowDiff;
	if (row1 > row2) {
		rowDiff = row1 - row2;
	}
	else {
		rowDiff = row2 - row1;
	}

	double length = sqrt((colDiff * colDiff) + (rowDiff * rowDiff));
	// round
	return (int)(length + .5);
}

// For each room this will find the distance to the closest room and return the average of those.
// This should be a decent measure of if rooms are close together but there are cases where it
// is not ideal, such as when there are separated clusters of rooms.
int Map::getAverageRoomDist() {
	int totalDist = 0;
	int numDist = 0;
	if (roomData.size() < 2) {
		return 0;
	}
	for (int x = 0; x < roomData.size(); x++) {
		int smallest = 0;
		int first = 1;
		for (int y = x + 1; y < roomData.size(); y++) {
			int dist = measureRoomDistance(roomData[x], roomData[y]);
			if (dist < smallest || first == 1) {
				smallest = dist;
				first = 0;
			}
		}
		totalDist += smallest;
		numDist++;
	}
	return totalDist / numDist;
}

int Map::getAverageRoomSize() {
	int totalSize = 0;
	for (int x = 0; x < roomData.size(); x++) {
		totalSize += roomData[x].xSize * roomData[x].ySize;
	}
	return totalSize / roomData.size();
}

int Map::getAverageHallLength() {
	int totalSize = 0;
	for (int x = 0; x < hallLengths.size(); x++) {
		totalSize += hallLengths[x];
	}
	return totalSize / hallLengths.size();
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
