#include "stdafx.h"
#include "Enemy.h"
#include "Global_Map.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Console.h"
#include <queue>
#include <vector>
#include "Monsters.h"
#include "Items.h"

Enemy::Enemy(){
	blocking = 1;
}

Enemy::~Enemy(){

}

// checks if the player is visible to the enemy
// BFS
int Enemy::sensePlayer_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start) {

	if (nodes.size() == 0) {
		return -1;
	}

	// get the current node
	int current_node = nodes.front();
	nodes.pop();
	int previous_node = parent_nodes.front();
	parent_nodes.pop();

	// player not found within distance
	if (distance == 0) {
		return -1;
	}

	if (global_map->map[current_node]->icon == global_map->player->icon) {
		// return the parent node
		return previous_node;
	}

	// add the surrounding nodes
	int x, next_node;
	for (x = 0; x < 4; x++) {
		if (x == 0) {
			next_node = current_node - global_map->size;
		}
		else if (x == 1) {
			next_node = current_node + global_map->size;
		}
		else if (x == 2) {
			next_node = current_node - 1;
		}
		else{
			next_node = current_node + 1;
		}

		// add unvisited, non-blocking nodes to the search list
		// throws error
		if (global_map->map[next_node]->blocking == 0 || (global_map->map[next_node]->icon == '@')){ //|| global_map->map[next_node]->icon == global_map->player->icon) {
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
	int new_distance = distance - 1;
	int result = sensePlayer_BFS(new_distance, nodes, parent_nodes, visited, start);
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

// returns the direction the enemy should move, 0 if no movement
int Enemy::sensePlayer() {

	std::queue<int> nodes;
	std::queue<int> parent_nodes;
	std::vector<int> visited;
	int moveTo;
	// add the current location to the node queue and visited array 
	nodes.push(this->location);
	parent_nodes.push(-1);
	visited.push_back(this->location);

	// distance should be square of view distance
	int distance = this->viewDistance;
	distance = distance * distance;

	// call the BFS recursive function with these data structures
	moveTo = sensePlayer_BFS(distance, nodes, parent_nodes, visited, this->location);

	// move if the player is adjacent
	if (global_map->map[location - global_map->size]->icon == '@') {
		return 1;
	}
	else if (global_map->map[location + global_map->size]->icon == '@') {
		return 2;
	}
	else if (global_map->map[location - 1]->icon == '@') {
		return 3;
	}
	else if (global_map->map[location + 1]->icon == '@') {
		return 4;
	}

	// return the direction to be moved
	if (moveTo == this->location - global_map->size) {
		return 1;
	}
	else if (moveTo == this->location + global_map->size) {
		return 2;
	}
	else if (moveTo == this->location - 1) {
		return 3;
	}
	else if (moveTo == this->location + 1) {
		return 4;
	}
	else {
		return 0;
	}
}

// moves the enemy in the given direction
int Enemy::Move(int direction) {

	int nextLocation;
	int lastLocation;
	int size = global_map->size;
	int x, y;
	
	onScreen(&x, &y);

	// up
	if (direction == 1) {
		
		nextLocation = location - size;

		if (global_map->map[nextLocation]->blocking == 0) {
			
			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = under;
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			under = new_under;

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[lastLocation]->icon);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[nextLocation]->icon);
			}

			return 1;
		}
		else {
			return 0;
		}
	}
	// down
	else if (direction == 2) {
		
		nextLocation = location + size;

		if (global_map->map[nextLocation]->blocking == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = under;
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			under = new_under;

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[lastLocation]->icon);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[nextLocation]->icon);
			}

			return 1;
		}
		else {
			return 0;
		}
	}
	// left
	else if (direction == 3) {
		
		nextLocation = location - 1;

		if (global_map->map[nextLocation]->blocking == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = under;
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			under = new_under;

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[lastLocation]->icon);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[nextLocation]->icon);
			}

			return 1;
		}
		else {
			return 0;
		}
	}
	// right
	else if (direction == 4) {
		
		nextLocation = location + 1;

		if (global_map->map[nextLocation]->blocking == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = under;
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			under = new_under;
			
			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[lastLocation]->icon);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, global_map->map[nextLocation]->icon);
			}

			return 1;
		}
		else {
			return 0;
		}
	}
	// invalid input
	else {
		return -1;
	}
}

void Enemy::attackPlayer() {
	global_map->player->takeDamage(strength);

	// create the event message
	std::string event("A ");
	event.append(this->name);
	event.append(" hits you for ");
	event.append(std::to_string(strength));
	event.append(" damage.");
	global_map->Add_Event(event);
	global_map->Draw_Events();
}

void Enemy::takeDamage(int amount) {
	// draw the attack symbol on the enemy
	flashChar(global_map->player->attack_char);

	// update health
	health = health - amount;
	
	// create damage event
	std::string event("A ");
	event.append(this->name);
	event.append(" takes ");
	event.append(std::to_string(amount));
	event.append(" damage");

	// death
	if (health <= 0) {
		int x, y;
		onScreen(&x, &y);
		health = 0;
		int i;
		// find the index of the dead enemy then remove from list and map
		for (i = 0; i < global_map->Enemy_List.size(); i++) {
			if (global_map->Enemy_List[i] == this) {
				break;
			}
		}
		// get loot
		int loot_index = rand() % 10;
		Tile* dropped_loot = new Tile();
		if (loot[loot_index] != -1) {
			delete dropped_loot;
			dropped_loot = Get_Item(location, loot[loot_index]);
			dropped_loot->under = under;
			under = dropped_loot;
		}
		global_map->Enemy_List.erase(global_map->Enemy_List.begin() + i);
		global_map->map[location] = under;
		// update the screen
		if (x != -1) {
			global_map->player->updateScreen(x, y, global_map->map[location]->icon);
		}
		event.append(" and dies.");
		global_map->Dead_Enemies.push_back(this);
	}
	else {
		event.append(". It has ");
		event.append(std::to_string(health));
		event.append(" health left.");
	}
	global_map->Add_Event(event);
	global_map->Draw_Events();
}

void Enemy::enemyTurn() {
	
	int moveResult;
	int was_viewed, is_viewed;
	int direction = sensePlayer();

	if (direction != 0) {
		// check if currently in view

		moveResult = this->Move(direction);
		
		// update the screen if in view
		if (moveResult == 1) {
			// check if in view now

			//update the screen depending on if monster was viewed and is now viewed
		}
		// interact with player
		if (moveResult == 0) {
			this->attackPlayer();
		}
		//error
		if (moveResult == -1) {
		}
	}
}

// returns coordinates if the enemy is on the screen
void Enemy::onScreen(int* X, int* Y) {
	
	int view_start = global_map->player->view_start;
	int view_distance = global_map->player->view_distance;
	int size = global_map->size;
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

					*X = x * 2;
					*Y = y;
					return;
				}
			}
		}
	}

	*X = -1;
	*Y = -1;
	return;
}

int Enemy::Player_Attack(int damage) {
	takeDamage(damage);
	return 1;
}

// draws the input char and then redraws the original char
void Enemy::flashChar(char flash) {
	int x, y;

	onScreen(&x, &y);
	if (x != -1 && y != -1) {
		global_map->player->updateScreen(x, y, flash);
		// todo: put sleep function here
		global_map->player->updateScreen(x, y, global_map->map[location]->icon);
	}
}
