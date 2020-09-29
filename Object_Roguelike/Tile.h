#pragma once
#include <Windows.h>
#include <string>
#include "Texture.h"

class Tile{

public:
	// constructor and destructor
	Tile();
	Tile(char iconI, bool blockingI);
	~Tile();

	// interact functions
	virtual void Player_Interact();
	virtual void Player_Step();
	virtual void Spell_Interact(int damage, int effect, int intensity);

	// todo: create a player_attack function.
	// this should be called on the 3x3 attack grid when the player tries to move
	// will do nothing for most tiles but will be an attack for the enemy class
	virtual int Player_Attack(int damage);
	
	// todo: add a function to fetch extra tile info like item stats 
	//virtual std::string getExtraInfo();
	virtual void render(int x, int y, int color);

	// other functions
	void drawUnderInfo();
	void flash(int colorIn, int delay);
	// get screen coordinates
	void onScreen(int* X, int* Y);

	// properties
	bool blocking;
	bool border;
	char icon;
	
	// track what sprite should be used
	int sprite; // todo: refactor name to be wall specific
	int spritePath;
	int spriteType;
	int spriteSheetW;
	int color;
	
	std::string name;
	std::string description;

	// Tile under this one
	Tile* under;
};

