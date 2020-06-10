#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"

Tile::Tile() {
	blocking = 1;
	icon = ' ';
	under = nullptr;
}

Tile::Tile(char iconI, bool blockingI){
	blocking = blockingI;
	icon = iconI;
	under = nullptr;
}


Tile::~Tile(){
	delete under;
}

void Tile::Player_Interact() {

}

void Tile::Spell_Interact(int damage, int effect, int intensity) {

}

int Tile::Player_Attack(int damage) {
	return 0;
}
