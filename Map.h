#pragma once
#include "Tile.h"
#include "Player.h"
#include <vector>

class Map{

public:
	
	Map(int size, int total, int max, int min, bool overlap, int mapType, int level);
	~Map();

	std::vector<Tile*> map;
	Player* player;
	
	int size;
	int total_rooms;
	int actual_total_rooms;
	int max_room_size;
	int min_room_size;
	int room_overlap;
	int type;
	int level;

private:

	void Make_Special_Tunnel();
	void Make_Rooms();
	void Fill_Map();
	void Close_Map();
	void Tunnel();
	void Close_Special();
	void Clear_Special();
	void Fill_Dead_Ends();
	void Map_Generate();
	void Set_Exit();
	void Spawn_Enemies();
};

