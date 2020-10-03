
#include "Texture.h"

std::string tilePaths[NUMPATHS] = {
	"Dawnlike Tiles/Objects/Tile.png",
	"Dawnlike Tiles/Objects/Wall.png",
	"Dawnlike Tiles/Characters/Player0.png",
	"Dawnlike Tiles/Characters/Slime0.png",
	"Dawnlike Tiles/Items/MedWep.png",
	"Dawnlike Tiles/Items/Scroll.png",
	"Dawnlike Tiles/Items/ShortWep.png",
	"Dawnlike Tiles/Objects/Floor.png"
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

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(fonts_g[FONT_12], textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer_g, textSurface);
		if (texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return texture != NULL;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::render(int x, int y, SDL_Rect* clip = NULL) {
	SDL_Rect renderQuad = { x, y, width, height };

	int imageScale = TILE_SIZE / 16;

	if (clip != NULL) {
		renderQuad.w = clip->w * imageScale;
		renderQuad.h = clip->h * imageScale;
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

		// red tiles
		Texture* tileSetRed = new Texture();
		tileSetRed->loadFromFile(tilePaths[x]);
		tileSetRed->setColor(255, 0, 0);
		tileSets_g.push_back(tileSetRed);

		// cyan tiles
		Texture* tileSetCyan = new Texture();
		tileSetCyan->loadFromFile(tilePaths[x]);
		tileSetCyan->setColor(102, 255, 255);
		tileSets_g.push_back(tileSetCyan);
	}
}

void freeTilesets() {
	while (!tileSets_g.empty()) {
		delete tileSets_g.back();
		tileSets_g.pop_back();
	}
}
