#pragma once
#include "Tile.h"
#include "Weapon.h"
#include "Armour.h"
#include "Spell.h"
#include <list>

class Player : public Tile{

public:
	
	Player();
	~Player();

	// functions
	void turn();
	int inView(int buffer);
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
	bool receiveAttack(int damage, std::string name, int faction, Tile* source);
	void resetCamera();
	void drawInfoWindow();
	void drawUnderInfoWindow();
	void addEvent(std::string event);
	void drawEvents();
	void drawWeaponInfo();
	void drawMapInfo();
	void drawSpellInfo();
	void drawInfoString(std::string inputText);

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
	void setRoomBuffer(int ro);
	void setRoomOverlap(int ro);
	void setCurrentInfoWindow(int wi);


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
	int getRoomBuffer();
	int getRoomOverlap();
	int getCurrentInfoWindow();

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
	int slowTurns;
	int currentInfoWindow;
	int currentSpell;
	std::list<std::string> events;

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
	int roomBuffer;
	int roomOverlap;
};

