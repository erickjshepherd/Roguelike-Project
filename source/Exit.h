#pragma once
#include "Global_Map.h"
#include "Tile.h"

class Exit : public Tile {
public:
	Exit();
	~Exit();
	void playerStep();
};
