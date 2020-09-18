#pragma once

#include <string>
#include "SDLFuncs.h"

#define NUMPATHS 7
#define NUMCOLORS 2

#define CASTR 255
#define CASTG 153
#define CASTB 153

enum pathEnums {
	TILEPATH,
	WALLPATH,
	PLAYERPATH,
	SLIMEPATH,
	MEDWEPPATH,
	SCROLLPATH,
	SHORTWEPPATH
};

enum colorEnums {
	STANDARD,
	CAST
};

// paths to tile sets
extern std::string tilePaths[NUMPATHS];

class Texture {
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void free();
	void render(int x, int y, SDL_Rect* clip);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* texture;
	int width;
	int height;
};

extern std::vector<Texture*> tileSets_g;

void loadTileSets();

void freeTilesets();
