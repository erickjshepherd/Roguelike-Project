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
#include <thread>

Map* global_map;

int main(int argc, char* argv[]){
    
	srand((unsigned)time(0));

	// initialize SDL
	SDL_Init();

	// start the frame clock
	std::thread frameThread(&frameClock);

	// create the player character
	Player* PC = new Player();

	// get a new level
	PC->getNewLevel(1);

	while (PC->getQuit() == 0) {
		SDL_RenderPresent(renderer_g);
		PC->turn();
		SDL_RenderPresent(renderer_g);
		if (PC->getExtraTurns() == 0) {
			global_map->Enemy_Turn();
		}
		else {
			int playerET = PC->getExtraTurns();
			PC->setExtraTurns(playerET - 1);
		}
	}
	
	// stop the frame clock
	drawFrame_g = -1;
	frameThread.join();

	freeTilesets();
	SDL_Close();

	return 0;
}

