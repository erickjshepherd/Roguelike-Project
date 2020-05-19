#pragma once
#include <string>
class Tile{

public:
	// constructor and destructor
	Tile();
	Tile(char iconI, bool blockingI);
	~Tile();

	// interact functions
	virtual void Player_Interact();

	// todo: create a player_attack function.
	// this should be called on the 3x3 attack grid when the player tries to move
	// will do nothing for most tiles but will be an attack for the enemy class
	virtual int Player_Attack(int damage);
	
	// todo: add a function to fetch extra tile info like item stats 
	//virtual std::string getExtraInfo();

	// properties
	bool blocking;
	char icon;
	std::string name;
	std::string description;

	// Tile under this one
	Tile* under;
};

