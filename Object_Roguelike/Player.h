#pragma once
#include "Tile.h"
#include "Weapon.h"
#include "Armour.h"
#include "Spell.h"

class Player : public Tile{

public:
	
	Player();
	~Player();

	// location and screen variables
	int location;
	int consoleX, consoleY;
	int view_distance;
	int view_start;
	int underDescribed;

	// player state variables
	int health;
	int strength;

	// player objects
	Weapon* weapon;
	Armour* head;
	Armour* chest;
	Armour* legs;
	Spell* spell1;
	Spell* spell2;
	Spell* spell3;

	// misc variables
	// todo: pull this from weapon/spell
	char attack_char;

	// map variables
	int size;
	int totalRooms;
	int maxRoomSize;
	int minRoomSize;
	int maxTunnelSize;
	int minTunnelSize;
	int roomOverlap;

	int getStart(int type);
	void turn();
	bool inView();
	void updateScreen(int X, int Y, char out);
	void setCoordinates();
	void Draw_Player_View();
	void drawStats(int line);
	void clearStats(int line);
	void drawUnderInfo();
	void takeDamage(int amount);
	//void Draw_Town();
	void Get_New_Level(int level);
	int Move(int direction);
	int getDamage(int x, int y);
	bool attack(int direction);
	void flashChar(char flash);
	void decreaseSpellCD();
	int selectSpell();
	void flashSpells();

	int getMapSize();
	int getTotalRooms();
	int getMaxRoomSize();
	int getMinRoomSize();
	int getMaxTunnelSize();
	int getMinTunnelSize();
	int getRoomOverlap();
};

