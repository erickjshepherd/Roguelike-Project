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
	virtual void playerInteract();
	virtual void playerStep();
	virtual void spellInteract(int damage, int effect, int effectDamage, int intensity);
	virtual int playerAttack(int damage);
	virtual bool enemyAttack(int damage, std::string name);

	// other functions
	void drawUnderInfo();
	void flash(int colorIn, int delay);
	void onScreen(int* X, int* Y);
	virtual void render(int x, int y, int color);
	virtual void resetColor();

	// setters
	void setBlocking(bool b);
	void setBorder(bool b);
	void setIcon(char i);
	void setName(std::string n);
	void setDescription(std::string d);
	void setSpriteVersion(int sv);
	void setSpritePath(int sp);
	void setSprite(int s);
	void setSpriteSheetW(int w);
	void setColor(int c);
	void setUnder(Tile* u);

	// getters
	bool getBlocking();
	bool getBorder();
	char getIcon();
	std::string getName();
	std::string getDescription();
	int getSpriteVersion();
	int getSpritePath();
	int getSprite();
	int getSpriteSheetW();
	int getColor();
	Tile* getUnder();

private:
	// properties
	bool blocking;
	bool border;
	char icon;
	std::string name;
	std::string description;

	// sprite properties
	int spriteVersion;	// some objects have multiple sprites. This selects which to use and replaces the sprite variable. ex: walls and floors
	int spritePath;		// path to the sprite sheet file
	int sprite;			// location on the spritesheet
	int spriteSheetW;	// width of the sprite sheet
	int color;			// color of the sprite

	// Tile under this one
	Tile* under;
};

