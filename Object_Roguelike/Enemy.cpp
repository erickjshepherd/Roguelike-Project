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
	charmed = 0;
	movement = 2;
	attacks = 1;
	turnPeriod = 1;
	turnCount = 0;
	location = 0;
	idleMove = 0;
	prevDirection = 0;
	setColor(STANDARD);
	setFaction(ENEMY1);
}

Enemy::~Enemy(){

}

// checks if the player is visible to the enemy
// BFS
int Enemy::senseTarget_BFS(int distance, std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start, int searchBlocking, int target) {

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

	if (global_map->map[current_node]->getFaction() == target) {
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
		if ((global_map->map[next_node]->getIcon() != '#' && searchBlocking == 1) || global_map->map[next_node]->getBlocking() == 0 || global_map->map[next_node]->getFaction() == target){
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
	int result = senseTarget_BFS(new_distance, nodes, parent_nodes, visited, start, searchBlocking, target);
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
int Enemy::senseTarget() {

	// choose the target
	int target = getTarget();

	// move if the target is adjacent
	int direction = inRange(location, target);
	if (direction != 0) {
		return direction;
	}

	// search for the target
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
	moveTo = senseTarget_BFS(distance, nodes, parent_nodes, visited, this->location, 0, target);
	if (moveTo == -1) {
		nodesSecond.push(this->location);
		pNodesSecond.push(-1);
		visitedSecond.push_back(this->location);
		moveTo = senseTarget_BFS(distance, nodesSecond, pNodesSecond, visitedSecond, this->location, 1, target);
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
		//int playerSize = global_map->player->getMapSize();
		//global_map->player->setMapSize(playerSize - 1);
	}
	else {
		event.append(". It has ");
		event.append(std::to_string(health));
		event.append(" health left.");

		// update screen
		int x, y;
		onScreen(&x, &y);
		if (x != -1) {
			global_map->player->updateScreen(x, y, -1);
		}
	}
	global_map->Add_Event(event);
	global_map->Draw_Events();
}

void Enemy::enemyTurn() {

	// Handle all the status conditions every round regardless of if the enemy takes a turn
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
	// Handle charmed status
	if (charmed == 0) {
		setFaction(ENEMY1);
	}
	else {
		charmed--;
	}
	// Handle slow status
	if (slowed > 0) {
		slowed--;
		if (slowed % 2 == 1) {
			resetColor();
			return;
		}
	}

	// Take a turn?
	turnCount++;
	if (turnCount < turnPeriod) {
		return;
	}
	else {
		turnCount = 0;
	}

	int attackCount = 0;
	for (int x = 0; x < movement; x++) {
		int attacked = 0;

		// get the direction to move
		int direction = senseTarget();

		// Handle scared status
		if (scared > 0) {
			scared--;
			direction = reverseDirection(direction);
		}

		// target was sensed
		int moveSuccess;
		if (direction != 0) {
			
			// only attack if there is something in range
			int target = getTarget();
			int attackDir = inRange(location, target);
			if (attackDir != 0) {
				// try attacking
				for (int y = 0; y < attacks; y++) {
					if (attackCount < attacks) {
						if (attack(attackDir)) {
							attackCount++;
							attacked = 1;
						}
					}
				}
			}
		}
		// idle movement
		else {
			if (idleMove) {
				direction = getIdleDirection(prevDirection);
			}
		}
		// move if attack fails
		if (attacked == 0 && direction != 0) {
			moveSuccess = Move(direction);
			if (moveSuccess == 1) {
				prevDirection = direction;
			}
			onScreen(&consoleX, &consoleY);
		}
	}

	resetColor();
}

bool Enemy::receiveAttack(int damage, std::string name, int faction) {
	if (faction != getFaction() && faction != NEUTRAL) {
		takeDamage(damage);
		return 1;
	}
	else {
		return 0;
	}
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
		setFaction(PLAYER);
		charmed = length;
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
	else if (charmed > 0) {
		setColor(PINK);
	}
	else if (scared > 0) {
		setColor(PURPLE);
	}
	else if (slowed > 0) {
		setColor(DARKGREEN);
	}
	else {
		setColor(STANDARD);
	}

	if (oldColor != getColor()) {
		render(consoleX * tileSize_g, consoleY * tileSize_g, -1);
	}
}

bool Enemy::attack(int direction) {
	int x, y, target, priority, success;

	// up
	success = 0;
	if (direction == 1) {
		for (priority = 1; priority < 10; priority++) {
			target = location - (3 * global_map->size) - 1;
			for (x = 0; x < 3; x++) {
				target += x * global_map->size;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction())) {
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
					if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction())) {
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
					if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction())) {
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
					if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction())) {
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

void Enemy::renderHealth() {
	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[WALLPATH * NUMCOLORS + STANDARD];

	// get the location on the sheet for green
	int greenClipX = 0;
	int greenClipY = 0;
	int XOffset = 12;
	int YOffset = 1;
	greenClipX += XOffset * TILE_SOURCE_SIZE;
	greenClipY += YOffset * TILE_SOURCE_SIZE;

	// get the location on the sheet for red
	int redClipX = 0;
	int redClipY = 0;
	XOffset = 15;
	YOffset = 0;
	redClipX += XOffset * TILE_SOURCE_SIZE;
	redClipY += YOffset * TILE_SOURCE_SIZE;

	// get the size of the health bar
	int tileSize = TILE_SOURCE_SIZE;
	int healthBarR = (health * tileSize) / maxHealth;
	if ((health * tileSize) % maxHealth > 0) {
		healthBarR++;
	}
	int damageBar = tileSize - healthBarR;

	// get the coordinates
	int x, y;
	onScreen(&x, &y);
	x *= tileSize_g;
	y *= tileSize_g;

	// set up the green clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = greenClipX;
	clip->y = greenClipY;
	clip->w = healthBarR;
	clip->h = tileSize / 8;

	spriteSheet->render(x, y, clip);

	// set up the red clip
	clip->x = redClipX;
	clip->y = redClipY;
	clip->w = damageBar;
	clip->h = tileSize / 8;

	x += healthBarR * (tileSize_g / TILE_SOURCE_SIZE);
	spriteSheet->render(x, y, clip);

	delete clip;
}

// virtual function for rendering tile sprite to screen
// inputs: x coordinate to render at, y coordinate t orender at, the color to render
void Enemy::render(int x, int y, int colorIn) {

	// set the view port
	SDL_RenderSetViewport(renderer_g, &mapView_g);

	// render the under tile with the same color
	if (getUnder() != nullptr) {
		getUnder()->render(x, y, colorIn);
	}

	// -1 is current color
	if (colorIn == -1) {
		colorIn = getColor();
	}
	// get the sprite sheet
	Texture* spriteSheet;
	if (drawFrame_g == 0) {
		spriteSheet = tileSets_g[getSpritePath() * NUMCOLORS + colorIn];
	}
	else if (drawFrame_g == 1 && tileSets2_g[getSpritePath() * NUMCOLORS + colorIn]->getWidth() != 0) {
		spriteSheet = tileSets2_g[getSpritePath() * NUMCOLORS + colorIn];
	}
	else {
		spriteSheet = tileSets_g[getSpritePath() * NUMCOLORS + colorIn];
	}

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	int typeXOffset = getSprite() % getSpriteSheetW();
	int typeYOffset = getSprite() / getSpriteSheetW();
	clipX += typeXOffset * TILE_SOURCE_SIZE;
	clipY += typeYOffset * TILE_SOURCE_SIZE;

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = TILE_SOURCE_SIZE;
	clip->h = TILE_SOURCE_SIZE;

	spriteSheet->render(x, y, clip);

	delete clip;

	if (health != maxHealth) {
		renderHealth();
	}
}

int Enemy::reverseDirection(int direction) {
	int newDirection = 0;
	int newLocation = 0;
	// try moving in the opposite direction
	if (direction == 1) {
		newDirection = 2;
		newLocation = location + global_map->size;
	}
	else if (direction == 2) {
		newDirection = 1;
		newLocation = location - global_map->size;
	}
	else if (direction == 3) {
		newDirection = 4;
		newLocation = location + 1;
	}
	else if (direction == 4) {
		newDirection = 3;
		newLocation = location - 1;
	}
	// if that fails move in a random direction
	if (global_map->map[newLocation]->getBlocking() == 1) {
		int randDirection = (rand() % 4) + 1;
		while (randDirection == direction && randDirection != newDirection) {
			randDirection = (rand() % 4) + 1;
		}
		return randDirection;
	}
	else {
		return newDirection;
	}
}

int Enemy::getIdleDirection(int direction) {
	int newDirection, newLocation;
	
	// get possible moves
	std::vector<int> moves;
	for (int x = 1; x <= 4; x++) {
		int newLocation = getNewLocation(x);
		if (global_map->map[newLocation]->getBlocking() == 0) {
			moves.push_back(x);
		}
	}

	// return 0 if can't move
	int numMoves = moves.size();
	if (numMoves == 0) {
		return 0;
	}

	// if input direction is 0 get a random direction
	if (direction == 0) {
		return moves[rand() % numMoves];
	}
	else {
		// try moving forward
		newDirection = getRelatedDirection(direction, 1);
		for (int x = 0; x < numMoves; x++) {
			if (moves[x] == newDirection) {
				return newDirection;
			}
		}

		// try moving left/right
		int firstRelation = (rand() % 2) + 3;
		int secondRelation;
		if (firstRelation == 3) {
			secondRelation = 4;
		}
		else {
			secondRelation = 3;
		}
		newDirection = getRelatedDirection(direction, firstRelation);
		for (int x = 0; x < numMoves; x++) {
			if (moves[x] == newDirection) {
				return newDirection;
			}
		}
		newDirection = getRelatedDirection(direction, secondRelation);
		for (int x = 0; x < numMoves; x++) {
			if (moves[x] == newDirection) {
				return newDirection;
			}
		}

		// try moving backward
		newDirection = getRelatedDirection(direction, 2);
		for (int x = 0; x < numMoves; x++) {
			if (moves[x] == newDirection) {
				return newDirection;
			}
		}
	}

	return 0;
}

// output: the damage the enemy deals with an attack
int Enemy::getDamage(int x, int y) {
	int damage = strength;
	damage += this->damage[x][y];
	return damage;
}

// return the direction of any valid attack targets
int Enemy::inRange(int location, int targetFaction) {
	int x, y, target, numHit, direction, maxHit;
	direction = 0;
	maxHit = 0;

	// up
	numHit = 0;
	target = location - (3 * global_map->size) - 1;
	for (x = 0; x < 3; x++) {
		target += x * global_map->size;
		for (y = 0; y < 3; y++) {
			if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] > 0) {
				if (global_map->map[target]->getFaction() == targetFaction) {
					numHit++;
				}
			}
			target++;
		}
		target = location - (3 * global_map->size) - 1;
	}
	if (numHit > maxHit) {
		direction = 1;
		maxHit = numHit;
	}
	// down
	numHit = 0;
	target = location + (3 * global_map->size) + 1;
	for (x = 0; x < 3; x++) {
		target -= x * global_map->size;
		for (y = 0; y < 3; y++) {
			if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] > 0) {
				if (global_map->map[target]->getFaction() == targetFaction) {
					numHit++;
				}
			}
			target--;
		}
		target = location + (3 * global_map->size) + 1;
	}
	if (numHit > maxHit) {
		direction = 2;
		maxHit = numHit;
	}
	// left
	numHit = 0;
	target = location - 3 + global_map->size;
	for (x = 0; x < 3; x++) {
		target += x;
		for (y = 0; y < 3; y++) {
			if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] > 0) {
				if (global_map->map[target]->getFaction() == targetFaction) {
					numHit++;
				}
			}
			target -= global_map->size;
		}
		target = location - 3 + global_map->size;
	}
	if (numHit > maxHit) {
		direction = 3;
		maxHit = numHit;
	}
	// right
	numHit = 0;
	target = location + 3 - global_map->size;
	for (x = 0; x < 3; x++) {
		target -= x;
		for (y = 0; y < 3; y++) {
			if (target >= 0 && target < (global_map->size*global_map->size) && hit[x][y] > 0) {
				if (global_map->map[target]->getFaction() == targetFaction) {
					numHit++;
				}
			}
			target += global_map->size;
		}
		target = location + 3 - global_map->size;
	}
	if (numHit > maxHit) {
		direction = 4;
		maxHit = numHit;
	}
	return direction;
}

void Enemy::attackInit(int hit[3][3], int damage[3][3]) {
	int x, y;
	for (x = 0; x < 3; x++) {
		for (y = 0; y < 3; y++) {
			this->hit[x][y] = hit[x][y];
			this->damage[x][y] = damage[x][y];
		}
	}
}

int Enemy::getTarget() {
	int target;
	if (getFaction() == ENEMY1) {
		target = PLAYER;
	}
	else if (getFaction() == PLAYER) {
		target = ENEMY1;
	}
	return target;
}

// inputs (1-indexed):
// directions: up, down, left, right
// relations: forward, backward, left, right
int Enemy::getRelatedDirection(int direction, int relation) {
	if (direction == 1) {
		if (relation == 1) {
			return 1;
		}
		else if (relation == 2) {
			return 2;
		}
		else if (relation == 3) {
			return 3;
		}
		else if (relation == 4) {
			return 4;
		}
	}
	if (direction == 2) {
		if (relation == 1) {
			return 2;
		}
		else if (relation == 2) {
			return 1;
		}
		else if (relation == 3) {
			return 4;
		}
		else if (relation == 4) {
			return 3;
		}
	}
	if (direction == 3) {
		if (relation == 1) {
			return 3;
		}
		else if (relation == 2) {
			return 4;
		}
		else if (relation == 3) {
			return 2;
		}
		else if (relation == 4) {
			return 1;
		}
	}
	if (direction == 4) {
		if (relation == 1) {
			return 4;
		}
		else if (relation == 2) {
			return 3;
		}
		else if (relation == 3) {
			return 1;
		}
		else if (relation == 4) {
			return 2;
		}
	}
}

int Enemy::getNewLocation(int direction) {
	int newLocation;
	if (direction == 1) {
		newLocation = location - global_map->size;
	}
	else if (direction == 2) {
		newLocation = location + global_map->size;
	}
	else if (direction == 3) {
		newLocation = location - 1;
	}
	else if (direction == 4) {
		newLocation = location + 1;
	}
	else {
		newLocation = 0;
	}
	return newLocation;
}
