#include "Armour.h"
#include "Player.h"
#include "Global_Map.h"

Armour::Armour() {
	type = 0;
	defense = 0;
	health = 0;
	rooms = 0;
	roomSize = 0;
	totalSize = 0;
	blocking = 0;
}

Armour::~Armour() {

}

// equip based on type
void Armour::Player_Interact() {

}