#include "stdafx.h"
#include "Exit.h"

Exit::Exit() {
	setBlocking(0);
	setIcon('E');
	setName("exit");
	setDescription("The exit!");
	setSpritePath(TILEPATH);
	setSpriteSheetW(8);
	setSprite(30);
}

Exit::~Exit() {

}

void Exit::playerStep() {
	global_map->player->setExtraTurns(1);
	global_map->player->getNewLevel(global_map->level + 1);
}
