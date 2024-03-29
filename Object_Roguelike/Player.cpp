#include "stdafx.h"
#include "Player.h"
#include "Global_Map.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <list>
#include "Items.h"
#include "GUI.h"
#include "SubMenus.h"
#include "Shared.h"

// player class
Player::Player(){
	
	// set player state
	setBlocking(1);
	setIcon('@');
	health = 100;
	strength = 10;
	extraTurns = 0;
	quit = 0;
	slowTurns = 0;
	setFaction(PLAYER);
	currentInfoWindow = EVENTS_W;
	
	// set player sprite info
	setSpritePath(PLAYERPATH);
	setSpriteSheetW(8);
	setSprite(30);
	setColor(STANDARD);

	// set initial map generation variables
	size = 40;
	totalRooms = 25;
	maxRoomSize = 14;
	minRoomSize = 8;
	maxTunnelSize = 4;
	minTunnelSize = 1;
	roomBuffer = 0;
	roomOverlap = 0;

	// set map and location variables
	viewDistance = 10;
	
	weapon = new Weapon(0, 0, DAGGER, location);
}

// player turn function
// reads key input and selects a direction to move
int Player::turn(int input) {

	damaged = 0;
	int nextLocation = 0;
	int move_success, attack_success;
	int prevFrame = -1;
	int direction = -1;
	int validKey = 0;

	// keep screen coordinates up to date
	setCoordinates();

	if (input == EVENT_KEY_UP) {
		direction = UP;
	}
	else if (input == EVENT_KEY_DOWN) {
		direction = DOWN;
	}
	else if (input == EVENT_KEY_LEFT) {
		direction = LEFT;
	}
	else if (input == EVENT_KEY_RIGHT) {
		direction = RIGHT;
	}
	else if (input == EVENT_KEY_1) {

	}
	else if (input == EVENT_KEY_2) {

	}
	else if (input == EVENT_KEY_3) {

	}
	else if (input == EVENT_KEY_ENTER) {
		getUnder()->playerInteract();
		return PLAYER_S;
	}
	else if (input == EVENT_KEY_TAB) {
		currentInfoWindow = nextInfoWindow(currentInfoWindow);
		drawInfoWindow();
		return PLAYER_S;
	}

	// handle freeze condition
	if (frozenLength > 0) {
		frozenLength--;
		if (frozenLength == 0) {
			receiveAttack(frozenDamage, spellSource->getName(), spellSource->getFaction(), spellSource);
		}
		return ENEMY_S;
	}

	// try to attack and then try to move
	attack_success = attack(direction);
	if (attack_success == 0) {
		move_success = Move(direction);
	}
	
	// Interact if can't attack or move
	if (attack_success != 1 && move_success == -1 && input != 0) {

		int target = -1;

		if (direction == UP) {
			target = location - global_map->size;
		}
		else if (direction == DOWN) {
			target = location + global_map->size;
		}
		else if (direction == LEFT) {
			target = location - 1;
		}
		else if (direction == RIGHT) {
			target = location + 1;
		}
		int interactResult = global_map->map[target]->playerInteract();
		if (interactResult == -1) {
			slowTurns = 1;
		}
	}
	// Do post move actions
	else {
		if (global_map->map[location]->getUnder() != NULL) {
			global_map->map[location]->getUnder()->playerStep();
		}
	}
	
	// decrease spell cooldowns at the end of turn
	decreaseSpellCD();

	return ENEMY_S;
}

// is the player within the camera view?
// output: true if yes, false if no
int Player::inView(int buffer) {

	int x, y, xy;
	int position = -1;

	// get the width of the square to draw
	int view_size = (viewDistance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = viewStart;
			xy += y * global_map->size;
			xy += x;

			// don't read out of range
			if (xy >= 0 && xy < (global_map->size * global_map->size)) {

				if (global_map->map[xy] == this) {

					// top
					if (y < buffer) {
						position = 2;
					}
					// bottom
					if (y >= view_size - buffer) {
						position = 8;
					}
					// left
					if (x < buffer) {
						// top left
						if (position == 2) {
							position = 1;
						}
						// bottom left
						else if (position == 8) {
							position = 7;
						}
						else {
							position = 4;
						}
					}
					// right
					if (x >= view_size - buffer) {
						// top right
						if (position == 2) {
							position = 3;
						}
						// bottom right
						else if (position == 8) {
							position = 9;
						}
						else {
							position = 6;
						}
					}
					// center
					if (position == -1) {
						position = 5;
					}
				}
				// don't read past the border
				if (global_map->map[xy]->getBorder() && (xy % global_map->size) == (global_map->size - 1)) {
					break;
				}
			}
		}
	}

	return position;
}

// update the screen at specific coordinates
// inputs: X and Y are console coordinates, out is printed there
// todo: does this belong in the player class?
void Player::updateScreen(int X, int Y, int color) {

	SDL_RenderSetViewport(renderer_g, &mapView_g);

	short view_size = (short)(viewDistance * 2) + 1;

	short x = (short)X;
	short y = (short)Y;

	if (X >= view_size || X < 0) {
		return;
	}
	if (Y >= view_size || Y < 0) {
		return;
	}

	// update sprite
	int loc = viewStart;
	loc += X;
	loc += y * global_map->size;
	global_map->map[loc]->render(X * tileSize_g, Y * tileSize_g, color);
}

// set the player screen coordinates
// output: updates consoleX and consoleY of the player class
void Player::setCoordinates() {

	int x, y, xy;

	// get the width of the square to draw
	int view_size = (viewDistance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = viewStart;
			xy += y * global_map->size;
			xy += x;

			if (xy >= 0 && xy < (global_map->size * global_map->size)) {
				
				if (global_map->map[xy] == this) {

					consoleX = x;
					consoleY = y;
				}
			}
		}
	}
}

// draw the players view
// output: the area around the player is drawn to the console
void Player::drawPlayerView(int view) {

	int x, y, xy;

	// clear screen
	if (view == -1) {
		SDL_RenderClear(renderer_g);
	}
	if (view == 0 || view == -1) {
		clearMap();
		drawBackground(0);
		// get the width of the square to draw
		int view_size = (viewDistance * 2) + 1;

		for (y = 0; y < view_size; y++) {

			for (x = 0; x < view_size; x++) {

				xy = viewStart;
				xy += y * global_map->size;
				xy += x;

				if (xy >= 0 && xy < (global_map->size * global_map->size)) {

					// draw sprite
					global_map->map[xy]->render(x * tileSize_g, y * tileSize_g, -1);

					if (global_map->map[xy]->getBorder() && (xy % global_map->size) == (global_map->size - 1)) {
						break;
					}
				}
			}
		}
	}
	if (view == 1 || view == -1) {
		drawBackground(1);
		drawStats(-1);
	}
	if (view == 2 || view == -1) {
		drawBackground(2);
		drawInfoWindow();
	}
}

// create a new level
// input: 0 if random, 1 if town
void Player::getNewLevel(int level) {

	while (1) {
		Map* next = new Map(getMapSize(), getTotalRooms(), getMaxRoomSize(), getMinRoomSize(), getRoomBuffer(), getRoomOverlap(), 0, level, getMaxTunnelSize(), getMinTunnelSize());
		next->player = this;

		// delete the old map
		delete(global_map);

		// set up the new map and location
		global_map = next;
		location = global_map->playerStart;
		viewDistance = 10;
		consoleX = viewDistance * 2;
		consoleY = viewDistance;
		viewStart = location - viewDistance;
		viewStart = viewStart - (global_map->size * viewDistance);
		while (viewStart < 0) {
			viewStart += global_map->size;
		}
		while ((viewStart % global_map->size) > (location % global_map->size)) {
			viewStart++;
		}

		// set the player in the map
		setUnder(global_map->map[location]);
		global_map->map[location] = this;

		// check that the exit can be reached
		if (global_map->findExit(location)) {
			break;
		}
	}

	// update the screen
	drawPlayerView(-1);
}

// move the player in one of four directions
// input: 1 is up, 2 is down, 3 is left, 4 is right
int Player::Move(int direction) {

	int nextLocation;
	bool success = false;
	
	// set the view port
	SDL_RenderSetViewport(renderer_g, &mapView_g);
	int prevSection = inView(2);

	// get the input and move if able
	if (direction == UP) {

		nextLocation = location - global_map->size;

		if (global_map->map[location - global_map->size]->getBlocking() == 0) {

			// get the new under tile
			Tile* new_under = global_map->map[location - global_map->size];

			// update map
			global_map->map[location - global_map->size] = this;
			global_map->map[location] = getUnder();
			setUnder(new_under);
			location -= global_map->size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, getColor());
			setCoordinates();
			updateScreen(consoleX, consoleY, getColor());

			// update camera position and redraw screen
			int currentSection = inView(2);
			if (currentSection != prevSection) {
				resetCamera();
			}

			success = true;
		}
	}
	else if (direction == DOWN) {

		nextLocation = location + global_map->size;

		if (global_map->map[location + global_map->size]->getBlocking() == 0) {

			Tile* new_under = global_map->map[location + global_map->size];

			global_map->map[location + global_map->size] = this;
			global_map->map[location] = getUnder();
			setUnder(new_under);
			location += global_map->size;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, getColor());
			setCoordinates();
			updateScreen(consoleX, consoleY, getColor());

			// update camera position and redraw screen
			int currentSection = inView(2);
			if (currentSection != prevSection) {
				resetCamera();
			}

			success = true;
		}
	}
	else if (direction == LEFT) {

		nextLocation = location - 1;

		if (global_map->map[location - 1]->getBlocking() == 0) {

			Tile* new_under = global_map->map[location - 1];

			global_map->map[location - 1] = this;
			global_map->map[location] = getUnder();
			setUnder(new_under);
			location--;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, getColor());
			setCoordinates();
			updateScreen(consoleX, consoleY, getColor());

			// update camera position and redraw screen
			int currentSection = inView(2);
			if (currentSection != prevSection) {
				resetCamera();
			}

			success = true;
		}
	}
	else if (direction == RIGHT) {

		nextLocation = location + 1;

		if (global_map->map[location + 1]->getBlocking() == 0) {

			Tile* new_under = global_map->map[location + 1];

			global_map->map[location + 1] = this;
			global_map->map[location] = getUnder();
			setUnder(new_under);
			location++;

			// update console and console coordinates
			updateScreen(consoleX, consoleY, getColor());
			setCoordinates();
			updateScreen(consoleX, consoleY, getColor());

			// update camera position and redraw screen
			int currentSection = inView(2);
			if (currentSection != prevSection) {
				resetCamera();
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
	int damage = strength;
	if (this->weapon != NULL) {
		damage += this->weapon->damage[x][y];
	}
	return damage;
}

// handles logic for the player taking damage
// input: the amount of damage to take
void Player::takeDamage(int amount) {
	// only flash once per turn
	if (damaged != 1) {
		//flash(RED, 100);
		damaged = 1;
	}
	if (health < amount) {
		health = 0;
		// player is dead
	}
	else {
		health = health - amount;
	}
	drawStats(HEALTH);
}

// input: The line number that corrosponds to the stat. -1 draws all
void Player::drawStats(int line) {
	short view_size = (short)(viewDistance * 2) + 1;
	short x = 2 * view_size;
	short y;

	// add space between stats equal to half text height
	int textSpace = (getTextSpace() * 3) / 2;

	// set and clear the view port
	SDL_RenderSetViewport(renderer_g, &statsView_g);
	clearStats(line);

	// y is the line number for each stat
	// todo: create functions for each stat. cleaner
	for (y = 0; y < 15; y++) {
		if (y == HEALTH && (y == line || line == -1)) {
			std::string healthStr("Health: " + std::to_string(health));
			Texture text;
			text.loadFromRenderedText(healthStr, textColor_g, -1);
			text.render(0, HEALTH * textSpace, NULL);
		}
		else if (y == STRENGTH && (y == line || line == -1)) {
			std::string strengthStr("Strength: " + std::to_string(strength));
			Texture text;
			text.loadFromRenderedText(strengthStr, textColor_g, -1);
			text.render(0, STRENGTH * textSpace, NULL);
		}
		else if (y == FLOOR && (y == line || line == -1)) {
			std::string floorStr("Floor: " + std::to_string(global_map->level));
			Texture text;
			text.loadFromRenderedText(floorStr, textColor_g, -1);
			text.render(0, FLOOR * textSpace, NULL);
		}
		else if (y == ROOMS && (y == line || line == -1)) {
			std::string roomStr("Rooms: " + std::to_string(global_map->actual_total_rooms));
			Texture text;
			text.loadFromRenderedText(roomStr, textColor_g, -1);
			text.render(0, ROOMS * textSpace, NULL);
		}
		else if (y == WEAPON && (y == line || line == -1)) {
			std::string weaponName;
			if (weapon != NULL) {
				weaponName.append(weapon->getName());
			}
			else {
				weaponName.append("none");
			}
			std::string weaponStr("Weapon: " + weaponName);
			Texture text;
			text.loadFromRenderedText(weaponStr, textColor_g, -1);
			text.render(0, WEAPON * textSpace, NULL);
		}
		else if (y == HEAD && (y == line || line == -1)) {
			std::string headName;
			if (head != NULL) {
				headName.append(head->getName());
			}
			else {
				headName.append("none");
			}
			std::string headStr("Head: " + headName);
			Texture text;
			text.loadFromRenderedText(headStr, textColor_g, -1);
			text.render(0, HEAD * textSpace, NULL);
		}
		else if (y == CHEST && (y == line || line == -1)) {
			std::string chestName;
			if (chest != NULL) {
				chestName.append(chest->getName());
			}
			else {
				chestName.append("none");
			}
			std::string chestStr("Chest: " + chestName);
			Texture text;
			text.loadFromRenderedText(chestStr, textColor_g, -1);
			text.render(0, CHEST * textSpace, NULL);
		}
		else if (y == LEGS && (y == line || line == -1)) {
			std::string legName;
			if (legs != NULL) {
				legName.append(legs->getName());
			}
			else {
				legName.append("none");
			}
			std::string legStr("Legs: " + legName);
			Texture text;
			text.loadFromRenderedText(legStr, textColor_g, -1);
			text.render(0, LEGS * textSpace, NULL);
		}
		else if (y == SPELL1 && (y == line || line == -1)) {
			std::string spellName;
			if (spell1 != NULL) {
				spellName.append("Spell 1 (" + std::to_string(spell1->cdCount) + "): " + spell1->getName());
			}
			else {
				spellName.append("Spell 1: none");
			}
			Texture text;
			text.loadFromRenderedText(spellName, textColor_g, -1);
			text.render(0, SPELL1 * textSpace, NULL);
		}
		else if (y == SPELL2 && (y == line || line == -1)) {
			std::string spellName;
			if (spell2 != NULL) {
				spellName.append("Spell 2 (" + std::to_string(spell2->cdCount) + "): " + spell2->getName());
			}
			else {
				spellName.append("Spell 2: none");
			}
			Texture text;
			text.loadFromRenderedText(spellName, textColor_g, -1);
			text.render(0, SPELL2 * textSpace, NULL);
		}
		else if (y == SPELL3 && (y == line || line == -1)) {
			std::string spellName;
			if (spell3 != NULL) {
				spellName.append("Spell 3 (" + std::to_string(spell3->cdCount) + "): " + spell3->getName());
			}
			else {
				spellName.append("Spell 3: none");
			}
			Texture text;
			text.loadFromRenderedText(spellName, textColor_g, -1);
			text.render(0, SPELL3 * textSpace, NULL);
		}
		else if (y == INSPECT && (y == line || line == -1)) {
			std::string inspectStr;
			inspectStr.append("Inspection:");
			Texture text;
			text.loadFromRenderedText(inspectStr, textColor_g, -1);
			text.render(0, INSPECT * textSpace, NULL);
		}
	}
}

// input: line number to clear. -1 clears all
void Player::clearStats(int line) {
	short view_size = (short)(viewDistance * 2) + 1;
	short x = 2 * view_size;
	short y;
	int textSpace = (getTextSpace() * 3) / 2;
	COORD position;
	HANDLE handle;

	SDL_RenderSetViewport(renderer_g, &statsView_g);

	SDL_Rect rect;
	rect.x = 0;
	rect.h = textSpace;
	rect.w = statsView_g.w;

	// y is the line number for each stat
	// todo: create functions for each stat. cleaner
	for (y = 0; y < 15; y++) {
		if (y == HEALTH && (y == line || line == -1)) {
			rect.y = textSpace * HEALTH;
			clearRect(rect);
		}
		else if (y == STRENGTH && (y == line || line == -1)) {
			rect.y = textSpace * STRENGTH;
			clearRect(rect);
		}
		else if (y == FLOOR && (y == line || line == -1)) {
			rect.y = textSpace * FLOOR;
			clearRect(rect);
		}
		else if (y == ROOMS && (y == line || line == -1)) {
			rect.y = textSpace * ROOMS;
			clearRect(rect);
		}
		else if (y == WEAPON && (y == line || line == -1)) {
			rect.y = textSpace * WEAPON;
			clearRect(rect);
		}
		else if (y == HEAD && (y == line || line == -1)) {
			rect.y = textSpace * HEAD;
			clearRect(rect);
		}
		else if (y == CHEST && (y == line || line == -1)) {
			rect.y = textSpace * CHEST;
			clearRect(rect);
		}
		else if (y == LEGS && (y == line || line == -1)) {
			rect.y = textSpace * LEGS;
			clearRect(rect);
		}
		else if (y == SPELL1 && (y == line || line == -1)) {
			rect.y = textSpace * SPELL1;
			clearRect(rect);
		}
		else if (y == SPELL2 && (y == line || line == -1)) {
			rect.y = textSpace * SPELL2;
			clearRect(rect);
		}
		else if (y == SPELL3 && (y == line || line == -1)) {
			rect.y = textSpace * SPELL3;
			clearRect(rect);
		}
		else if (y == INSPECT && (y == line || line == -1)) {
			rect.y = textSpace * INSPECT;
			clearRect(rect);
		}
		else if (y == INSPECTINFO && (y == line || line == -1)) {
			rect.y = textSpace * INSPECTINFO;
			clearRect(rect);
		}
	}
}

// input: direction to attack
// output: if the attack worked
bool Player::attack(int direction) {
	int x, y, target, priority, success;
	// up
	success = 0;
	if (direction == UP) {
		for (priority = 1; priority < 10; priority++) {
			target = location - (3 * global_map->size) - 1;
			for (x = 0; x < 3; x++) {
				target += x * global_map->size;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction(), this)) {
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
	else if (direction == DOWN) {
		for (priority = 1; priority < 10; priority++) {
			target = location + (3 * global_map->size) + 1;
			for (x = 0; x < 3; x++) {
				target -= x * global_map->size;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction(), this)) {
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
	else if (direction == LEFT) {
		for (priority = 1; priority < 10; priority++) {
			target = location - 3 + global_map->size;
			for (x = 0; x < 3; x++) {
				target += x;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction(), this)) {
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
	else if (direction == RIGHT) {
		for (priority = 1; priority < 10; priority++) {
			target = location + 3 - global_map->size;
			for (x = 0; x < 3; x++) {
				target -= x;
				for (y = 0; y < 3; y++) {
					if (target >= 0 && target < (global_map->size*global_map->size) && weapon->hit[x][y] == priority) {
						if (global_map->map[target]->receiveAttack(getDamage(x, y), getName(), getFaction(), this)) {
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

// decreases the cooldown on all spells
void Player::decreaseSpellCD() {
	if (spell1 != NULL) {
		if (spell1->cdCount > 0) {
			spell1->cdCount--;
			drawStats(SPELL1);
		}
	}
	if (spell2 != NULL) {
		if (spell2->cdCount > 0) {
			spell2->cdCount--;
			drawStats(SPELL2);
		}
	}
	if (spell3 != NULL) {
		if (spell3->cdCount > 0) {
			spell3->cdCount--;
			drawStats(SPELL3);
		}
	}
}

// reads player input to select a spell
// output: spell number. -1 if the operation was cancelled
int Player::selectSpell() {
	int input = 0;
	int validKey = 0;

	while (validKey == 0) {
		validKey = 1;

		input = handleEvents();

		if (input == EVENT_QUIT) {
			quit = 1;
			return -1;
		}
		else if (input == EVENT_KEY_1) {
			return 1;
		}
		else if (input == EVENT_KEY_2) {
			return 2;
		}
		else if (input == EVENT_KEY_3) {
			return 3;
		}
		else if (input == EVENT_KEY_ESC) {
			return -1;
		}
		else if (input == EVENT_RESIZE) {
			drawPlayerView(-1);
			SDL_RenderPresent(renderer_g);
			validKey = 0;
		}
		else {
			validKey = 0;
		}
	}
}

// clears the map viewport on the screen
void Player::clearMap() {
	SDL_RenderSetViewport(renderer_g, &mapView_g);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = mapView_g.h;
	rect.w = mapView_g.w;
	clearRect(rect);
	drawBackground(0);
}

// handles being attacked. Can only take damage from non allied objects.
// input: damage to take, name of the attacker, faction of the attacker
// output: if the attack worked 
bool Player::receiveAttack(int damage, std::string name, int faction, Tile* source) {
	if (faction != getFaction() && faction != NEUTRAL) {
		takeDamage(damage);

		// create the event message
		std::string event("A ");
		event.append(name);
		event.append(" hits you for ");
		event.append(std::to_string(damage));
		event.append(" damage.");
		addEvent(event);
		drawInfoWindow();

		return 1;
	}
	else {
		return 0;
	}
}

// reset the camera to the upper left of the player
void Player::resetCamera() {
	viewStart = location;
	for (int x = 0; x < viewDistance; x++) {
		if (viewStart % global_map->size != 0) {
			viewStart--;
		}
	}
	for (int x = 0; x < viewDistance; x++) {
		if (viewStart >= global_map->size) {
			viewStart -= global_map->size;
		}
	}
	drawPlayerView(0);
	setCoordinates();
}

// draws one of the info windows in the bottom right
void Player::drawInfoWindow() {
	if (currentInfoWindow == EVENTS_W) {
		drawEvents();
	}
	else if (currentInfoWindow == UNDER_W) {
		drawUnderInfoWindow();
	}
	else if (currentInfoWindow == WEAPON_W) {
		drawWeaponInfo();
	}
	else if (currentInfoWindow == SPELL_W) {
		drawSpellInfo();
	}
	else if (currentInfoWindow == MAP_W) {
		drawMapInfo();
	}
}

void Player::addEvent(std::string event) {
	this->events.push_back(event);
	if (this->events.size() > NUM_EVENTS) {
		this->events.pop_front();
	}
}

void Player::drawEvents() {
	std::list<std::string>::iterator current;
	int textSpace = (getTextSpace() * 3) / 2;

	clearInfoWindow();
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

void Player::drawUnderInfoWindow() {
	// split the description into lines
	std::string underString = "Item Info\n\n" + getUnder()->getDescription();
	drawInfoString(underString);
}

void Player::drawWeaponInfo() {
	// split the description into lines
	std::string weaponString = "Weapon Info\n\n" + weapon->getName() + ":\n" + weapon->getDescription();
	drawInfoString(weaponString);
}

void Player::drawMapInfo() {
	// split the description into lines
	std::string mapString = "Map Generation Info\n\n";
	mapString += "Biome: " + global_map->biomeName + "\n";
	mapString += "Map Width/Height: " + std::to_string(size) + "\n";
	mapString += "Max Possible Rooms: " + std::to_string(totalRooms) + "\n";
	mapString += "Max Room Size: " + std::to_string(maxRoomSize) + "\n";
	mapString += "Min Room Size: " + std::to_string(minRoomSize) + "\n";
	mapString += "Room Buffer Distance: " + std::to_string(roomBuffer) + "\n";
	mapString += "Max Tunnel Segment Size: " + std::to_string(maxTunnelSize) + "\n";
	mapString += "Min Tunnel Segment Size: " + std::to_string(minTunnelSize) + "\n";
	mapString += "Room Overlap: " + std::to_string(roomOverlap) + "\n";
	drawInfoString(mapString);
}

void Player::drawSpellInfo() {
	// split the description into line
	std::string spellString = "Spell Info\n\n";
	if (currentSpell == 1) {
		spellString += spell1->getName() + ":\n" + spell1->getDescription();
	}
	else if (currentSpell == 2) {
		spellString += spell2->getName() + ":\n" + spell2->getDescription();
	}
	else if (currentSpell == 3) {
		spellString += spell3->getName() + ":\n" + spell3->getDescription();
	}
	drawInfoString(spellString);
}

void Player::drawInfoString(std::string inputText) {
	std::vector<std::string> lines;
	std::vector<std::string>::iterator current;
	int textSpace = (getTextSpace() * 3) / 2;
	int pos = 0;
	int prev = 0;
	while ((pos = inputText.find('\n', prev)) != std::string::npos)
	{
		lines.push_back(inputText.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// clear the info window
	clearInfoWindow();
	drawBackground(2);

	// draw the description lines
	int eventY = 0;
	SDL_RenderSetViewport(renderer_g, &eventsView_g);
	for (current = lines.begin(); current != lines.end(); current++) {
		Texture text;
		text.loadFromRenderedText(*current, textColor_g, -1);
		text.render(0, eventY, NULL);
		eventY += textSpace;
	}
}

// setters
void Player::setLocation(int loc) {
	location = loc;
}
void Player::setConsoleX(int x) {
	consoleX = x;
}
void Player::setConsoleY(int y) {
	consoleY = y;
}
void Player::setViewDistance(int vd) {
	viewDistance = vd;
}
void Player::setViewStart(int vs) {
	viewStart = vs;
}
void Player::setHealth(int h) {
	health = h;
}
void Player::setStrength(int s) {
	strength = s;
}
void Player::setExtraTurns(int et) {
	extraTurns = et;
}
void Player::setQuit(int q) {
	quit = q;
}
void Player::setDamaged(int d) {
	damaged = d;
}
void Player::setWeapon(Weapon* w) {
	weapon = w;
}
void Player::setHead(Armour* h) {
	head = h;
}
void Player::setChest(Armour* c) {
	chest = c;
}
void Player::setLegs(Armour* l) {
	legs = l;
}
void Player::setSpell1(Spell* s) {
	spell1 = s;
}
void Player::setSpell2(Spell* s) {
	spell2 = s;
}
void Player::setSpell3(Spell* s) {
	spell3 = s;
}
void Player::setMapSize(int ms) {
	size = ms;
}
void Player::setTotalRooms(int tr) {
	totalRooms = tr;
}
void Player::setMaxRoomSize(int mrs) {
	maxRoomSize = mrs;
}
void Player::setMinRoomSize(int mrs) {
	minRoomSize = mrs;
}
void Player::setMaxTunnelSize(int mts) {
	maxTunnelSize = mts;
}
void Player::setMinTunnelSize(int mts) {
	minTunnelSize = mts;
}
void Player::setRoomBuffer(int rb) {
	roomBuffer = rb;
}
void Player::setRoomOverlap(int ro) {
	roomOverlap = ro;
}
void Player::setCurrentInfoWindow(int wi) {
	currentInfoWindow = wi;
}

//getters
int Player::getLocation() {
	return location;
}
int Player::getConsoleX() {
	return consoleX;
}
int Player::getConsoleY() {
	return consoleY;
}
int Player::getViewDistance() {
	return viewDistance;
}
int Player::getViewStart() {
	return viewStart;
}
int Player::getHealth() {
	return health;
}
int Player::getStrength() {
	return strength;
}
int Player::getExtraTurns() {
	return extraTurns;
}
int Player::getQuit() {
	return quit;
}
int Player::getDamaged() {
	return damaged;
}
Weapon* Player::getWeapon() {
	return weapon;
}
Armour* Player::getHead() {
	return head;
}
Armour* Player::getChest() {
	return chest;
}
Armour* Player::getLegs() {
	return legs;
}
Spell* Player::getSpell1() {
	return spell1;
}
Spell* Player::getSpell2() {
	return spell2;
}
Spell* Player::getSpell3() {
	return spell3;
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
int Player::getRoomBuffer() {
	return roomBuffer;
}
int Player::getRoomOverlap() {
	return roomOverlap;
}
int Player::getCurrentInfoWindow() {
	return currentInfoWindow;
}

// destructor
Player::~Player(){


}
