#pragma once
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

	// properties
	bool blocking;
	char icon;

	// Tile under this one
	Tile* under;
};

