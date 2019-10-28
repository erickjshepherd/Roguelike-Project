#pragma once
class Tile{

public:
	// constructor and destructor
	Tile();
	Tile(char iconI, bool blockingI);
	~Tile();

	// interact functions
	virtual void Player_Interact();

	// properties
	bool blocking;
	char icon;

	// Tile under this one
	Tile* under;
};

