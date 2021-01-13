#include "stdafx.h"
#include "Tile.h"
#include "Global_Map.h"
#include "Texture.h"
#include <iostream>
#include <conio.h>

// constructor
Tile::Tile() {
	blocking = 1;
	border = 0;
	spriteVersion = -1;
	icon = ' ';
	under = nullptr;
	faction = NEUTRAL;
}

// constructor
// inputs: tile icon, blocking
Tile::Tile(char iconI, bool blockingI){
	blocking = blockingI;
	border = 0;
	spriteVersion = -1;
	icon = iconI;
	under = nullptr;
}

// destructor
Tile::~Tile(){
	try {
		delete under;
	}
	catch(int e){
	
	}
}

// virtual function for player interaction
int Tile::playerInteract() {
	return 0;
}

// virtual function for player stepping
// outputs info of the tile being stepped on
void Tile::playerStep() {
	if (under != NULL) {
		under->playerStep();
	}
	global_map->player->drawUnderInfo(); // todo: handle more of this here instead of in player class
}

// virtual function for spell interaction
// inputs: initial damage being done, spell effect, effect related damage, duration
void Tile::spellInteract(int damage, int effect, int effectDamage, int intensity) {

}

bool Tile::receiveAttack(int damage, std::string name, int faction) {
	return 0;
}

// virtual function for rendering tile sprite to screen
// inputs: x coordinate to render at, y coordinate t orender at, the color to render
void Tile::render(int x, int y, int colorIn) {

	// set the view port
	SDL_RenderSetViewport(renderer_g, &mapView_g);

	// render the under tile with the same color
	if (under != nullptr) {
		under->render(x, y, colorIn);
	}

	// -1 is current color
	if (colorIn == -1) {
		colorIn = color;
	}
	// get the sprite sheet
	Texture* spriteSheet;
	if (drawFrame_g == 0) {
		spriteSheet = tileSets_g[spritePath * NUMCOLORS + colorIn];
	}
	else if (drawFrame_g == 1 && tileSets2_g[spritePath * NUMCOLORS + colorIn]->getWidth() != 0) {
		spriteSheet = tileSets2_g[spritePath * NUMCOLORS + colorIn];
	}
	else {
		spriteSheet = tileSets_g[spritePath * NUMCOLORS + colorIn];
	}

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	int typeXOffset = sprite % spriteSheetW;
	int typeYOffset = sprite / spriteSheetW;
	clipX += typeXOffset * TILE_SOURCE_SIZE;
	clipY += typeYOffset * TILE_SOURCE_SIZE;

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = TILE_SOURCE_SIZE;
	clip->h = TILE_SOURCE_SIZE;

	spriteSheet->render(x, y, clip);

	delete clip;
}

// makes the tile flash a new color with a delay
// inputs: the color to flash, the delay
void Tile::flash(int colorIn, int delay) {
	int x, y;
	onScreen(&x, &y);
	x *= tileSize_g;
	y *= tileSize_g;
	if (x == -1) {
		return;
	}
	if (under != nullptr) {
		under->render(x, y, -1);
	}

	// get the sprite sheet
	Texture* spriteSheet = tileSets_g[spritePath * NUMCOLORS + colorIn];

	// get the location on the sheet
	int clipX = 0;
	int clipY = 0;
	int typeXOffset = sprite % spriteSheetW;
	int typeYOffset = sprite / spriteSheetW;
	clipX += typeXOffset * TILE_SOURCE_SIZE;
	clipY += typeYOffset * TILE_SOURCE_SIZE;

	// set up the clip
	SDL_Rect* clip = new SDL_Rect();
	clip->x = clipX;
	clip->y = clipY;
	clip->w = TILE_SOURCE_SIZE;
	clip->h = TILE_SOURCE_SIZE;

	SDL_RenderSetViewport(renderer_g, &mapView_g);
	spriteSheet->render(x, y, clip);
	SDL_RenderPresent(renderer_g);
	Sleep(delay);
	render(x, y, -1);
	SDL_RenderPresent(renderer_g);

	delete clip;
}

// returns coordinates if the tile is on the screen
// inputs: pointer to the x coordinate, pointer to the y coordinate
void Tile::onScreen(int* X, int* Y) {

	int view_start = global_map->player->getViewStart();
	int view_distance = global_map->player->getViewDistance();
	int size = global_map->size;
	int x, y, xy;

	// get the width of the square to draw
	int view_size = (view_distance * 2) + 1;

	for (y = 0; y < view_size; y++) {

		for (x = 0; x < view_size; x++) {

			xy = view_start;
			xy += y * size;
			xy += x;

			if (xy >= 0 && xy < (size * size)) {

				if (global_map->map[xy]->border && (xy % global_map->size) == (global_map->size - 1)) {
					break;
				}

				if (global_map->map[xy] == this) {

					*X = x;
					*Y = y;
					return;
				}
			}
		}
	}

	*X = -1;
	*Y = -1;
	return;
}

// draws the tile description to the screen
void Tile::drawUnderInfo() {
	// todo: consolidate the text scaling into a function
	int textSpace = (getTextSpace() * 3) / 2;

	global_map->player->clearStats(INSPECTINFO);

	if (under->description.length() <= 0) {
		return;
	}

	// set the view port
	SDL_RenderSetViewport(renderer_g, &statsView_g);

	// todo: move to draw_stats
	std::string infoStr;
	infoStr.append(under->description);
	Texture text;
	text.loadFromRenderedText(infoStr, textColor_g, -1);
	text.render(0, INSPECTINFO * textSpace, NULL);
}

// resets the color of the tile to it's normal state. ex: stops flashing
void Tile::resetColor() {
	color = STANDARD;
}

// setters
void Tile::setBlocking(bool b) {
	blocking = b;
}
void Tile::setBorder(bool b) {
	border = b;
}
void Tile::setIcon(char i) {
	icon = i;
}
void Tile::setName(std::string n) {
	name = n;
}
void Tile::setDescription(std::string d) {
	description = d;
}
void Tile::setSpriteVersion(int sv) {
	spriteVersion = sv;
}
void Tile::setSpritePath(int sp) {
	spritePath = sp;
}
void Tile::setSprite(int s) {
	sprite = s;
}
void Tile::setSpriteSheetW(int w) {
	spriteSheetW = w;
}
void Tile::setColor(int c) {
	color = c;
}
void Tile::setUnder(Tile* u) {
	under = u;
}
void Tile::setFaction(int f) {
	faction = f;
}

// getters
bool Tile::getBlocking() {
	return blocking;
}
bool Tile::getBorder() {
	return border;
}
char Tile::getIcon() {
	return icon;
}
std::string Tile::getName() {
	return name;
}
std::string Tile::getDescription() {
	return description;
}
int Tile::getSpriteVersion() {
	return spriteVersion;
}
int Tile::getSpritePath() {
	return spritePath;
}
int Tile::getSprite() {
	return sprite;
}
int Tile::getSpriteSheetW() {
	return spriteSheetW;
}
int Tile::getColor() {
	return color;
}
Tile* Tile::getUnder() {
	return under;
}
int Tile::getFaction() {
	return faction;
}
