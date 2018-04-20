#pragma once
class Tile{

public:
	// constructor and destructor
	Tile();
	Tile(char iconI, bool blockingI);
	~Tile();

	// properties
	bool blocking;
	char icon;

	// Tile under this one
	Tile* under;
};

