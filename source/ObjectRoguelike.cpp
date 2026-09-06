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
#include "GUI.h"
#include "SubMenus.h"
#include "Shared.h"

Map* global_map;

int main(int argc, char* argv[]){
    
	srand((unsigned)time(0));

	// initialize SDL
	SDL_Init();

	// initialize the menus
	initMenus();
	mainMenu* mainM = new mainMenu();

	if (!openMenu(mainM)) {
		int quit = 0;
		int state = PLAYER_S;

		// create the player character
		Player* PC = new Player();

		// get a new level
		PC->getNewLevel(1);

		while (quit == 0) {
			// advance the passive animation clock
			updateFrameClock();

			// get valid input //
			int validKey = 1;
			int eventValue;
			int direction;

			// handle events
			eventValue = handleEvents();
			if (eventValue == -1) {

			}
			else if (validPlayerInput(eventValue)) {
				// stop additional inputs and clear the event buffer
				filterInputEvents();
				clearEvents();
			}
			else if (eventValue == EVENT_QUIT) {
				quit = 1;
			}
			else if (eventValue == EVENT_KEY_ESC) {
				pauseMenu* menu = new pauseMenu();
				int menuRet = openMenu(menu);
				if (menuRet == 1) {
					quit = 1;
				}
				PC->drawPlayerView(-1);
			}
			else if (eventValue == EVENT_RESIZE) {

			}

			// player logic //
			if (validPlayerInput(eventValue) && state == PLAYER_S) {
				state = PC->turn(eventValue);
				
				// enable SDL input events
				resetFilter();
			}
			
			// enemy logic //
			if (state == ENEMY_S) {
				state = global_map->Enemy_Turn();
			}
			
			// display logic //
			// passive animations
			if (drawFrame_g != currentFrame_g) {
				drawFrame_g = currentFrame_g;
				PC->drawPlayerView(0); // move this to the SDLFuncs file
			}

			// active animations

			SDL_RenderPresent(renderer_g);
		}
	}
	
	// free menus
	freeMenus();

	freeTilesets();
	SDL_Close();

	return 0;
}

