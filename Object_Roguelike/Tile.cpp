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
