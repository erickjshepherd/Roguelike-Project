#include "stdafx.h"
#include "Enemy.h"
#include "Global_Map.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <queue>
#include <vector>
#include "Monsters.h"
#include "Items.h"

Enemy::Enemy(){
	setBlocking(1);
	frozenLength = 0;
	frozenDamage = 0;
	burnedLength = 0;
	burnedDamage = 0;
	slowed = 0;
	scared = 0;
	allied = 0;
	setColor(STANDARD);
}

Enemy::~Enemy(){

}

// checks if the player is visible to the enemy
// BFS
int Enemy::sensePlayer_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start, int searchBlocking) {

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

	if (global_map->map[current_node]->getIcon() == global_map->player->getIcon()) {
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
		if ((global_map->map[next_node]->getIcon() != '#' && searchBlocking == 1) || global_map->map[next_node]->getBlocking() == 0 || global_map->map[next_node]->getIcon() == '@'){
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
	int result = sensePlayer_BFS(new_distance, nodes, parent_nodes, visited, start, searchBlocking);
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

	// move if the player is adjacent
	if (global_map->map[location - global_map->size]->getIcon() == '@') {
		return 1;
	}
	else if (global_map->map[location + global_map->size]->getIcon() == '@') {
		return 2;
	}
	else if (global_map->map[location - 1]->getIcon() == '@') {
		return 3;
	}
	else if (global_map->map[location + 1]->getIcon() == '@') {
		return 4;
	}


	// search for the player
	std::queue<int> nodes, nodesSecond;
	std::queue<int> parent_nodes, pNodesSecond;
	std::vector<int> visited, visitedSecond;
	int moveTo;
	// add the current location to the node queue and visited array 
	nodes.push(this->location);
	parent_nodes.push(-1);
	visited.push_back(this->location);

	// distance should be square of view distance
	int distance = this->viewDistance;
	distance = distance * distance;

	// call the BFS recursive function with these data structures
	// search once ignoring blocking tiles
	// If the player is not found include blocking tiles for the next best path
	moveTo = sensePlayer_BFS(distance, nodes, parent_nodes, visited, this->location, 0);
	if (moveTo == -1) {
		nodesSecond.push(this->location);
		pNodesSecond.push(-1);
		visitedSecond.push_back(this->location);
		moveTo = sensePlayer_BFS(distance, nodesSecond, pNodesSecond, visitedSecond, this->location, 1);
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

		if (global_map->map[nextLocation]->getBlocking() == 0) {
			
			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = getUnder();
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			setUnder(new_under);

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
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

		if (global_map->map[nextLocation]->getBlocking() == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = getUnder();
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			setUnder(new_under);

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
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

		if (global_map->map[nextLocation]->getBlocking() == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = getUnder();
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			setUnder(new_under);

			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
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

		if (global_map->map[nextLocation]->getBlocking() == 0) {

			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = getUnder();
			lastLocation = location;
			location = nextLocation;

			// set the new under tile
			setUnder(new_under);
			
			// if enemy was on screen update the old and new tile
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
			}
			onScreen(&x, &y);
			if (x != -1) {
				global_map->player->updateScreen(x, y, -1);
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
	event.append(getName());
	event.append(" hits you for ");
	event.append(std::to_string(strength));
	event.append(" damage.");
	global_map->Add_Event(event);
	global_map->Draw_Events();
}

void Enemy::takeDamage(int amount) {
	// flash the enemy sprite
	flash(RED, 100);

	// update health
	health = health - amount;
	
	// create damage event
	std::string event("A ");
	event.append(getName());
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
			dropped_loot->setUnder(getUnder());
			setUnder(dropped_loot);
		}
		global_map->map[location] = getUnder();

		// update the screen
		if (x != -1) {
			global_map->player->updateScreen(x, y, -1);
		}
		event.append(" and dies.");
		setUnder(nullptr);
		global_map->Dead_Enemies.push_back(this);
		global_map->Enemy_List[i] = NULL;

		// test
		int playerSize = global_map->player->getMapSize();
		global_map->player->setMapSize(playerSize--);
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

	// Handle frozen + burn
	if (frozenLength > 0 && burnedLength > 0) {
		frozenLength = 0;
		frozenDamage = 0;
		burnedLength = 0;
		burnedDamage = 0;
	}

	// Handle frozen status
	if (frozenLength > 0) {
		frozenLength--;
		if (frozenLength == 0) {
			takeDamage(frozenDamage);
			if (health <= 0) {
				return;
			}
			frozenDamage = 0;
		}
		resetColor();
		return;
	}
	// Handle burn status
	if (burnedLength > 0) {
		global_map->map[location - global_map->size]->spellInteract(burnedDamage, BURN, burnedDamage, burnedLength / 2);
		global_map->map[location + global_map->size]->spellInteract(burnedDamage, BURN, burnedDamage, burnedLength / 2);
		global_map->map[location - 1]->spellInteract(burnedDamage, BURN, burnedDamage, burnedLength / 2);
		global_map->map[location + 1]->spellInteract(burnedDamage, BURN, burnedDamage, burnedLength / 2);
		takeDamage(burnedDamage);
		if (health <= 0) {
			return;
		}
		burnedLength--;
		if (burnedLength == 0) {
			burnedDamage = 0;
		}
	}
	if (slowed > 0) {
		slowed--;
		if (slowed % 2 == 1) {
			resetColor();
			return;
		}
	}

	int direction = sensePlayer();

	// Handle scared status
	if (slowed > 0) {
		if (direction == 1 || direction == 3) {
			direction++;
		}
		else if (direction == 2 || direction == 4) {
			direction--;
		}

	}

	if (direction != 0) {
		// check if currently in view

		moveResult = Move(direction);
		onScreen(&consoleX, &consoleY);
		
		// update the screen if in view
		if (moveResult == 1) {
			// check if in view now

			//update the screen depending on if monster was viewed and is now viewed
		}
		// interact with player
		if (moveResult == 0) {
			attackPlayer();
		}
		//error
		if (moveResult == -1) {
		}
	}

	resetColor();
}

int Enemy::playerAttack(int damage) {
	takeDamage(damage);
	return 1;
}

void Enemy::spellInteract(int damage, int effect, int effectDamage, int length) {
	
	takeDamage(damage);
	if (health <= 0) {
		return;
	}

	if (effect == FREEZE) {
		std::string event("A ");
		event.append(getName());
		event.append(" has been frozen for ");
		event.append(std::to_string(length));
		event.append(" turns");
		global_map->Add_Event(event);
		global_map->Draw_Events();
		frozenLength = length;
		frozenDamage = effectDamage;
	}
	if (effect == BURN) {
		if (burnedLength == 0) {
			std::string event("A ");
			event.append(getName());
			event.append(" has been burned (");
			event.append(std::to_string(length));
			event.append(")");
			global_map->Add_Event(event);
			global_map->Draw_Events();
			burnedLength = length;
			burnedDamage = effectDamage;
		}
	}
	if (effect == SLOW) {
		std::string event("A ");
		event.append(getName());
		event.append(" has been slowed for ");
		event.append(std::to_string(length));
		event.append(" turns");
		global_map->Add_Event(event);
		global_map->Draw_Events();
		slowed = length;
	}
	if (effect == SCARE) {
		std::string event("A ");
		event.append(getName());
		event.append(" has been scared for ");
		event.append(std::to_string(length));
		event.append(" turns");
		global_map->Add_Event(event);
		global_map->Draw_Events();
		scared = length;
	}
	if (effect == CHARM) {
		std::string event("A ");
		event.append(getName());
		event.append(" has been charmed for ");
		event.append(std::to_string(length));
		event.append(" turns");
		global_map->Add_Event(event);
		global_map->Draw_Events();
		allied = length;
	}
	resetColor();
}

void Enemy::resetColor() {
	int oldColor = getColor();

	// listed in order of priority
	if (frozenLength > 0) {
		setColor(CYAN);
	}
	else if (burnedLength > 0) {
		setColor(ORANGE);
	}
	else if (allied > 0) {
		
	}
	else if (scared > 0) {
	
	}
	else if (slowed > 0) {
	
	}
	else {
		setColor(STANDARD);
	}

	if (oldColor != getColor()) {
		render(consoleX * TILE_SIZE, consoleY * TILE_SIZE, -1);
	}
}
