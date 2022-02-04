#pragma once
#include <Windows.h>
#include <string>
#include "Texture.h"

enum factionEnums {
	NEUTRAL,
	PLAYER,
	ENEMY1
};

class Tile{

public:
	// constructor and destructor
	Tile();
	Tile(char iconI, bool blockingI, bool room);
	~Tile();

	// effects
	int frozenLength;
	int frozenDamage;
	int burnedLength;
	int burnedDamage;
	int slowed;
	int scared;
	int charmed;
	Tile* spellSource;

	// interact functions
	virtual int playerInteract();
	virtual void playerStep();
	virtual void spellInteract(int damage, int effect, int effectDamage, int intensity, int direction, Tile* source);
	virtual bool receiveAttack(int damage, std::string name, int faction, Tile* source);

	// other functions
	void drawUnderInfo();
	void flash(int colorIn, int delay);
	void flashDir(int dir, int delay);
	void onScreen(int* X, int* Y);
	virtual void render(int x, int y, int color);
	virtual void resetColor();

	// setters
	void setBlocking(bool b);
	void setBorder(bool b);
	void setRoom(bool r);
	void setIcon(char i);
	void setName(std::string n);
	void setDescription(std::string d);
	void setSpriteVersion(int sv);
	void setSpritePath(int sp);
	void setSprite(int s);
	void setSpriteSheetW(int w);
	void setColor(int c);
	void setUnder(Tile* u);
	void setFaction(int f);

	// getters
	bool getBlocking();
	bool getBorder();
	bool getRoom();
	char getIcon();
	std::string getName();
	std::string getDescription();
	int getSpriteVersion();
	int getSpritePath();
	int getSprite();
	int getSpriteSheetW();
	int getColor();
	Tile* getUnder();
	int getFaction();

private:
	// properties
	bool blocking;
	bool border;
	bool room;
	char icon;
	std::string name;
	std::string description;
	int faction;

	// sprite properties
	int spriteVersion;	// some objects have multiple sprites. This selects which to use and replaces the sprite variable. ex: walls and floors
	int spritePath;		// path to the sprite sheet file
	int sprite;			// location on the spritesheet
	int spriteSheetW;	// width of the sprite sheet
	int color;			// color of the sprite

	// Tile under this one
	Tile* under;
};

