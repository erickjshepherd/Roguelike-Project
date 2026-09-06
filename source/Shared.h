#pragma once

enum dirEnums {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum turnEnums {
	PLAYERT,
	ENEMYT
};

enum stateEnums {
	MENU_S,
	PLAYER_S,
	ENEMY_S,
	ANIMATION_S
};

extern int turn_g;
