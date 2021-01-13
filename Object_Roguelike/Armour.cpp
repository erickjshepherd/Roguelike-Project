#include "Armour.h"
#include "Player.h"
#include "Global_Map.h"

Armour::Armour() {
	type = 0;
	defense = 0;
	health = 0;
	totalRooms = 0;
	maxRoomSize = 0;
	minRoomSize = 0;
	totalSize = 0;
	setBlocking(0);
}

Armour::~Armour() {

}

// equip based on type
int Armour::playerInteract() {
	return 0;
}