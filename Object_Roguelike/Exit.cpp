#include "Exit.h"

Exit::Exit() {
	blocking = 0;
	icon = 'E';
	name = "exit";
	description = "The exit!";
	spritePath = TILEPATH;
	spriteSheetW = 8;
	spriteType = 30;
}

Exit::~Exit() {

}

void Exit::Player_Step() {
	global_map->player->setExtraTurns(1);
	global_map->player->getNewLevel(global_map->level + 1);
}
