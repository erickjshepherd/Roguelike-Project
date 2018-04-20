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

Enemy::Enemy(){

}


Enemy::~Enemy(){

}

// checks if the player is visible to the enemy
// conical search, or BFS, or dijkstras?
int Enemy::sensePlayer() {


}

// moves the enemy in the given direction
void Enemy::Move(int direction) {

	int nextLocation;
	int size = global_map->size;

	// up
	if (direction == 1) {
		
		nextLocation = location - size;

		if (global_map->map[nextLocation]->blocking == 0) {
			
			Tile* new_under = global_map->map[nextLocation];

			// update map
			global_map->map[nextLocation] = this;
			global_map->map[location] = under;
			location = nextLocation;

			// set the new under tile
			under = new_under;
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
			location = nextLocation;

			// set the new under tile
			under = new_under;
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
			location = nextLocation;

			// set the new under tile
			under = new_under;
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
			location = nextLocation;

			// set the new under tile
			under = new_under;
		}
	}
	// invalid input
	else {
	}
}