#pragma once
#include "Tile.h"
#include "Weapon.h"

class Player : public Tile{

public:
	
	Player();
	~Player();

	// location and screen variables
	int location;
	int consoleX, consoleY;
	int view_distance;
	int view_start;

	// player state variables
	int health;
	int strength;

	// player objects
	Weapon* weapon;

	// misc variables
	// todo: pull this from weapon/spell
	char attack_char;

	// map variables
	int size;
	int total_rooms;
	int max_room_size;
	int min_room_size;
	int room_overlap;

	int getStart(int type);
	void turn();
	bool inView();
	void updateScreen(int X, int Y, char out);
	void setCoordinates();
	void Draw_Player_View();
	void drawStats(int line);
	void takeDamage(int amount);
	//void Draw_Town();
	void Get_New_Level(int level);
	int Move(int direction);
	int getDamage(int x, int y);
	bool attack(int direction);
};

