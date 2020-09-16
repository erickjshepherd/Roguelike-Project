#pragma once
#include "global_map.h"
#include "Tile.h"

class Exit : public Tile {
public:
	Exit();
	~Exit();
	void Player_Step();
};
