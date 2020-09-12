#pragma once

#include <string>
#include "SDLFuncs.h"

#define NUMPATHS 5

// paths to tile sets
std::string tilePaths[NUMPATHS] = {
	"Dawnlike Tiles/Objects/Tile.png",
	"Dawnlike Tiles/Objects/Wall.png",
	"Dawnlike Tiles/Characters/Player0.png",
	"Dawnlike Tiles/Characters/Slime0.png",
	"Dawnlike Tiles/Items/MedWep.png"
};

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
