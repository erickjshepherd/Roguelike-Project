#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"

Tile::Tile() {
	blocking = 1;
	border = 0;
	icon = ' ';
	under = nullptr;
}

Tile::Tile(char iconI, bool blockingI){
	blocking = blockingI;
	border = 0;
	icon = iconI;
	under = nullptr;
}

Tile::~Tile(){
	try {
		delete under;
	}
	catch(int e){
	
	}
}

void Tile::Player_Interact() {

}

void Tile::Spell_Interact(int damage, int effect, int intensity) {

}

int Tile::Player_Attack(int damage) {
	return 0;
}

void Tile::render(int x, int y) {

}
