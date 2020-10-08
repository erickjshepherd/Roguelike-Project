#pragma once

#include <string>
#include "SDLFuncs.h"

#define CASTR 255
#define CASTG 153
#define CASTB 153

#define NUMPATHS 8
enum pathEnums {
	TILEPATH,
	WALLPATH,
	PLAYERPATH,
	SLIMEPATH,
	MEDWEPPATH,
	SCROLLPATH,
	SHORTWEPPATH,
	FLOORPATH
};

#define NUMCOLORS 8
enum colorEnums {
	STANDARD,
	CAST,
	RED,
	CYAN,
	ORANGE,
	DARKGREEN,
	PURPLE,
	PINK
};

enum wallEnums {
	BRICK1_W,
	BRICK2_W,
	BRICK3_W,
	BRICK4_W,
	DIRT1_W,
	DIRT2_W,
	DIRT3_W,
	DIRT4_W,
	WCRYSTAL1_W,
	WCRYSTAL2_W,
	WCRYSTAL3_W,
	WCRYSTAL4_W,
	YSMOOTH1_W,
	YSMOOTH2_W,
	YSMOOTH3_W,
	YSMOOTH4_W,
	WOOD1_W,
	WOOD2_W,
	WOOD3_W,
	WOOD4_W,
	GRAVEL1_W,
	GRAVEL2_W,
	GRAVEL3_W,
	GRAVEL4_W,
	YCRYSTAL1_W,
	YCRYSTAL2_W,
	YCRYSTAL3_W,
	YCRYSTAL4_W,
	WSMOOTH1_W,
	WSMOOTH2_W,
	WSMOOTH3_W,
	WSMOOTH4_W,
	MINE1_W,
	MINE2_W,
	MINE3_W,
	MINE4_W,
	DIRT5_W,
	DIRT6_W,
	DIRT7_W,
	DIRT8_W,
	GCRYSTAL1_W,
	GCRYSTAL2_W,
	GCRYSTAL3_W,
	GCRYSTAL4_W,
	ICE1_W,
	ICE2_W,
	ICE3_W,
	ICE4_W
};

enum floorEnums {
	BRICK1_F,
	BRICK2_F,
	BRICK3_F,
	BRICK4_F,
	SPECK1_F,
	SPECK2_F,
	SPECK3_F,
	SPECK4_F,
	DIRT1_F,
	DIRT2_F,
	DIRT3_F,
	DIRT4_F,
	GRASS1_F,
	GRASS2_F,
	GRASS3_F,
	GRASS4_F,
	BRICK5_F,
	BRICK6_F,
	BRICK7_F,
	BRICK8_F,
	BLANK1_F,
	BLANK2_F,
	BLANK3_F,
	BLANK4_F,
	ROCK1_F,
	ROCK2_F,
	ROCK3_F,
	ROCK4_F,
	ICE1_F,
	ICE2_F,
	ICE3_F,
	ICE4_F
};

// paths to tile sets
extern std::string tilePaths[NUMPATHS];

class Texture {
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
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
