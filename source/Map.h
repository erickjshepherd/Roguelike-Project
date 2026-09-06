#pragma once
#include "Tile.h"
#include "Player.h"
#include <vector>
#include <string>
#include "Enemy.h"

#define NUM_EVENTS 5

enum biomeEnums {
	CRYSTAL_CAVE,
	ICE_CAVE,
	SUMMIT,
	MOUNTAIN,
	BASEMENT,
	CATACOMB,
	SEWER,
	MINE,
	VILLAGE,
	FIELD,
	CAVE,
	FOREST,
	HOUSE,
	CASTLE,
	CRYSTAL_CAVE_2,
	CITY
};

struct roomInfo {
	int xSize;
	int ySize;
	int center;
	int topLeft;
};

class Map{

public:
	
	Map(int size, int total, int max, int min, int buffer, bool overlap, int mapType, int level, int maxTunnel, int minTunnel);
	~Map();

	std::vector<Tile*> map;
	std::vector<int> specialTunnelY;
	std::vector<roomInfo> roomData;
	std::vector<int> hallLengths;
	std::vector<Enemy*> Enemy_List;
	std::vector<Enemy*> Dead_Enemies;
	Player* player;
	
	int size;
	int total_rooms;
	int actual_total_rooms;
	int maxRoomSize;
	int minRoomSize;
	int maxTunnelSize;
	int minTunnelSize;
	int roomBuffer;
	int room_overlap;
	int type;
	int level;
	int playerStart;
	int biome;
	std::string biomeName;

	int Enemy_Turn();
	int findExit_BFS(std::queue<int> &nodes, std::queue<int> &parent_nodes, std::vector<int> &visited, int start);
	int findExit(int start);
	int measurePointDistance(int x, int y);

private:

	void Make_Special_Tunnel();
	void Make_Rooms();
	void Fill_Map();
	void Close_Map();
	void Tunnel();
	void Close_Special();
	void Clear_Special();
	void Fill_Dead_Ends();
	int Map_Generate();
	int Set_Exit();
	void Spawn_Enemies();
	void Spawn_Items();
	void convertToClasses();
	void setWallSprites();
	void setFloorSprites();
	void clearRoomBuffers();
	int getOpenLocation(int roomOnly);
	int placePlayerStart();
	void clearPlayerStart();
	int determineBiome();
	int getWallSet();
	int getFloorSet();
	int measureRoomDistance(roomInfo r1, roomInfo r2);
	int getAverageRoomDist();
	int getAverageRoomSize();
	int getAverageHallLength();
};

