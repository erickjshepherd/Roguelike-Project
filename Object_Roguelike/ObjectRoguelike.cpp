#include "stdafx.h"
#include "Player.h"
#include "Map.h"
#include "Global_Map.h"
#include "Tile.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "SDLFuncs.h"

Map* global_map;

int main(int argc, char* argv[]){
    
	srand((unsigned)time(0));

	// initialize SDL
	SDL_Init();
	// load sprite sheets
	loadTileSets();

	// create the player character
	Player* PC = new Player();

	// get a new level
	PC->Get_New_Level(1);

	while (PC->quit == 0) {
		SDL_RenderPresent(renderer_g);
		PC->turn();
		SDL_RenderPresent(renderer_g);
		if (PC->extraTurns == 0) {
			global_map->Enemy_Turn();
		}
		else {
			PC->extraTurns--;
		}
	}

	freeTilesets();
	SDL_Close();

	return 0;
}

