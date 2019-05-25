#include "Player.h"
#include "Map.h"
#include "Global_Map.h"
#include "Tile.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <string>

Map* global_map;

int main(){
    
	srand((unsigned)time(0));

	// create the player character
	Player* PC = new Player();

	// get a new level
	PC->Get_New_Level(1);

	while (1) {
		PC->turn();
		global_map->Enemy_Turn();
	}

	return 0;
}

