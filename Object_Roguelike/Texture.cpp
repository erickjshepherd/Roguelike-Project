
#include "Texture.h"

std::string tilePaths[NUMPATHS] = {
	"Dawnlike Tiles/Objects/Tile.png",
	"Dawnlike Tiles/Objects/Wall.png",
	"Dawnlike Tiles/Characters/Player0.png",
	"Dawnlike Tiles/Characters/Slime0.png",
	"Dawnlike Tiles/Items/MedWep.png",
	"Dawnlike Tiles/Items/Scroll.png",
	"Dawnlike Tiles/Items/ShortWep.png"
};

std::vector<Texture*> tileSets_g;

Texture::Texture() {
	width = 0;
	height = 0;
	texture = NULL;
}

Texture::~Texture() {
	free();
}

void Texture::free() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

bool Texture::loadFromFile(std::string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		return 1;
	}
	else {
		//SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0xFF, 0xFF));
		texture = SDL_CreateTextureFromSurface(renderer_g, tempSurface);
		if (texture == NULL) {
			return 1;
		}
		else {
			width = tempSurface->w;
			height = tempSurface->h;
		}
		SDL_FreeSurface(tempSurface);
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::render(int x, int y, SDL_Rect* clip = NULL) {
	SDL_Rect renderQuad = { x, y, width, height };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer_g, texture, clip, &renderQuad);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

void loadTileSets() {
	int x;
	for (x = 0; x < NUMPATHS; x++) {
		// standard tiles
		Texture* tileSet = new Texture();
		tileSet->loadFromFile(tilePaths[x]);
		tileSets_g.push_back(tileSet);

		// cast preview tiles
		Texture* tileSetCast = new Texture();
		tileSetCast->loadFromFile(tilePaths[x]);
		tileSetCast->setColor(CASTR, CASTG, CASTB);
		tileSets_g.push_back(tileSetCast);
	}
}

void freeTilesets() {
	while (!tileSets_g.empty()) {
		delete tileSets_g.back();
		tileSets_g.pop_back();
	}
}
