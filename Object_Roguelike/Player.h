#pragma once
#include "Tile.h"
#include "Weapon.h"
#include "Armour.h"
#include "Spell.h"

class Player : public Tile{

public:
	
	Player();
	~Player();

	// functions
	int getStart(int type);
	void turn();
	bool inView();
	void updateScreen(int X, int Y, int color);
	void setCoordinates();
	void drawPlayerView(int view);
	void drawStats(int line);
	void clearStats(int line);
	void takeDamage(int amount);
	void getNewLevel(int level);
	int Move(int direction);
	int getDamage(int x, int y);
	bool attack(int direction);
	void decreaseSpellCD();
	int selectSpell();
	void clearMap();

	// setters
	void setLocation(int loc);
	void setConsoleX(int x);
	void setConsoleY(int y);
	void setViewDistance(int vd);
	void setViewStart(int vs);
	void setHealth(int h);
	void setStrength(int s);
	void setExtraTurns(int et);
	void setQuit(int q);
	void setDamaged(int d);
	void setWeapon(Weapon* w);
	void setHead(Armour* h);
	void setChest(Armour* c);
	void setLegs(Armour* l);
	void setSpell1(Spell* s);
	void setSpell2(Spell* s);
	void setSpell3(Spell* s);
	void setMapSize(int ms);
	void setTotalRooms(int tr);
	void setMaxRoomSize(int mrs);
	void setMinRoomSize(int mrs);
	void setMaxTunnelSize(int mts);
	void setMinTunnelSize(int mts);
	void setRoomOverlap(int ro);


	// getters
	int getLocation();
	int getConsoleX();
	int getConsoleY();
	int getViewDistance();
	int getViewStart();
	int getHealth();
	int getStrength();
	int getExtraTurns();
	int getQuit();
	int getDamaged();
	Weapon* getWeapon();
	Armour* getHead();
	Armour* getChest();
	Armour* getLegs();
	Spell* getSpell1();
	Spell* getSpell2();
	Spell* getSpell3();
	int getMapSize();
	int getTotalRooms();
	int getMaxRoomSize();
	int getMinRoomSize();
	int getMaxTunnelSize();
	int getMinTunnelSize();
	int getRoomOverlap();

private:
	// location and screen variables
	int location;
	int consoleX, consoleY;
	int viewDistance;
	int viewStart;

	// player state variables
	int health;
	int strength;
	int extraTurns;
	int quit;
	int damaged; // damaged this turn

	// player objects
	Weapon* weapon;
	Armour* head;
	Armour* chest;
	Armour* legs;
	Spell* spell1;
	Spell* spell2;
	Spell* spell3;

	// map variables
	int size;
	int totalRooms;
	int maxRoomSize;
	int minRoomSize;
	int maxTunnelSize;
	int minTunnelSize;
	int roomOverlap;
};

