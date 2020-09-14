#pragma once

#include <string>
#include "SDLFuncs.h"

#define NUMPATHS 7
#define TILEPATH 0
#define WALLPATH 1
#define PLAYERPATH 2
#define SLIMEPATH 3
#define MEDWEPPATH 4
#define SCROLLPATH 5
#define SHORTWEPPATH 6

// paths to tile sets
extern std::string tilePaths[NUMPATHS];

class Texture {
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
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
