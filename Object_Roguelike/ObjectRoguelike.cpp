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

	// test png drawing
	SDL_Texture* testpng = NULL;
	loadImage(testpng, "Dawnlike Tiles/Objects/Tile.png");
	SDL_RenderClear(renderer_g);
	SDL_RenderCopy(renderer_g, testpng, NULL, NULL);
	SDL_RenderPresent(renderer_g);

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

