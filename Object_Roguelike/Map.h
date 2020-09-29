#pragma once
#include "Tile.h"
#include "Player.h"
#include <vector>
#include <list>
#include <string>
#include "Enemy.h"

#define NUM_EVENTS 5

class Map{

public:
	
	Map(int size, int total, int max, int min, bool overlap, int mapType, int level, int maxTunnel, int minTunnel);
	~Map();

	std::vector<Tile*> map;
	std::vector<int> specialTunnelY;
	std::vector<Enemy*> Enemy_List;
	std::list<std::string> events;
	std::vector<Enemy*> Dead_Enemies;
	Player* player;
	
	int size;
	int total_rooms;
	int actual_total_rooms;
	int max_room_size;
	int min_room_size;
	int maxTunnelSize;
	int minTunnelSize;
	int room_overlap;
	int type;
	int level;

	void Enemy_Turn();
	void Add_Event(std::string event);
	void Draw_Events();
	int findExit_BFS(std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start);
	int findExit(int start);

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
	void Spawn_Items();
	void convertToClasses();
	void setWallSprites();
	void clearEvents();

};

